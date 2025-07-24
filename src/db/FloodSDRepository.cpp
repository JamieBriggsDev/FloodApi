//
// Created by Jamie Briggs on 23/07/2025.
//

#include "FloodSDRepository.h"
#include <Arduino.h>
#include <SD.h>
#include <SPI.h>
#include <string>
#include <vector>
#include "FloodSchema.h"
#include "def_pin_outs.h"
#include "logger/def_logger_factory.h"
long FloodSDRepository::findDatePosition(File& file, const char* startDate) const
{
  char buffer[BUFFER_SIZE];
  long left = 0;
  long right = file.size() - 1;
  long result = 0; // Default to start of file

  while (left < right)
  {
    long mid = left + (right - left) / 2;

    // Seek to nearest newline before mid point
    file.seek(mid);
    if (mid > 0)
    {
      // Read until we find a newline to align with record boundary
      while (mid > 0 && file.read() != '\n')
      {
        mid--;
        file.seek(mid);
      }
      if (mid > 0)
        mid++; // Move past the newline
    }
    file.seek(mid);

    // Read the line at this position
    int bytesRead = file.readBytesUntil('\n', buffer, BUFFER_SIZE - 1);
    buffer[bytesRead] = '\0';

    // Extract date from the buffer (assuming date is first field)
    char lineDate[11]; // YYYY-MM-DD\0
    memcpy(lineDate, buffer, 10);
    lineDate[10] = '\0';

    int comparison = strcmp(lineDate, startDate);

    if (comparison == 0)
    {
      // Exact match found
      return mid;
    }
    else if (comparison < 0)
    {
      // Date in file is earlier than target date
      left = file.position();
    }
    else
    {
      // Date in file is later than target date
      right = mid - 1;
      result = mid; // Keep track of this position as it might be the closest match
    }
  }

  // If we didn't find an exact match, return the position
  // of the first record that's not later than the target date
  return result;
}
void FloodSDRepository::init()
{
  SPI.begin();

  if (!SD.begin(MICRO_SD_CS_PIN))
  {
    LOG.error("SD card not found");
    throw new std::runtime_error("SD card not found");
  };
}

std::vector<RiverReading> FloodSDRepository::getRiverReadings(const char* startDate, uint16_t page,
                                                              uint8_t pageSize) const
{
  std::vector<RiverReading> readings;
  File file = SD.open(RIVER_LEVELS_FILE, FILE_READ);
  if (!file)
    return readings;

  // Find the starting position for the date
  long pos = findDatePosition(file, startDate);
  file.seek(pos);

  char buffer[BUFFER_SIZE];
  uint16_t skip = (page - 1) * pageSize;
  uint8_t count = 0;

  // Skip records based on pagination
  while (skip-- > 0 && file.available())
  {
    file.readBytesUntil('\n', buffer, BUFFER_SIZE);
  }

  // Read requested number of records
  while (count < pageSize && file.available())
  {
    int bytesRead = file.readBytesUntil('\n', buffer, BUFFER_SIZE - 1);
    buffer[bytesRead] = '\0';

    // Skip the header row if we're at the beginning of the file
    if (strstr(buffer, "TimeStamp,Level") != nullptr)
    {
      continue;
    }

    // Parse CSV line into RiverReading
    char timestamp[21]; // Enough space for ""YYYY-MM-DD HH:mm:ss""
    float level;

    if (sscanf(buffer, "\"%[^\"]\"%f", timestamp, &level) == 2)
    {
      RiverReading reading{.timestamp = timestamp, .level = level};
      readings.push_back(reading);
      count++;
    }
  }

  file.close();
  return readings;
}
std::vector<RainfallReading> FloodSDRepository::getStationRainfallReadings(const char* stationName,
                                                                           const char* startDate, uint16_t page,
                                                                           uint8_t pageSize) const
{
  std::vector<RainfallReading> readings;
  return readings;
}
