//
// Created by Jamie Briggs on 17/07/2025.
//

#include "../../src/db/FloodRepository.h"

#include <SD.h>
#include <SD_MMC.h>
#include <SPI.h>
#include <WiFiClient.h>
#include <mutex>
#include <sstream>
#include <string>
#include <cstdlib>

#include "FloodSchema.h"
#include "MySQL_Connection.h"
#include "MySQL_Cursor.h"
#include "def_pin_outs.h"
#include "def_wifi_settings.h"
#include "logger/def_logger_factory.h"

#define READ_ALL -1
#define PAGE_OFFSET 1

WiFiClient client;            // Use this for WiFi instead of EthernetClient
MySQL_Connection conn((Client *)&client);


void FloodRepository::init()
{
  LOG.info("Initializing FloodRepository");

  LOG.info_f("Connecting to database: %s@%s:%d",DB_NAME, DB_HOSTNAME.toString(), DB_PORT);
  if (conn.connect(DB_HOSTNAME, DB_PORT, DB_USERNAME, DB_PASSWORD, DB_NAME))
  {
    delay(1000);
    LOG.debug("Still trying to connect");
  }
  LOG.info_f("Connected to database!");

  LOG.info("Completed initialization for FloodRepository");
}


std::vector<RiverReading> FloodRepository::getRiverReadings(const char* startDate, uint16_t page, uint8_t pageSize) const
{
  std::vector<RiverReading> result;

  // Step 1: Create a cursor
  MySQL_Cursor *cur_mem = new MySQL_Cursor(&conn);

  // Step 2: Execute the query
  std::stringstream sql;
  // Casting pageSize to int as uint8_t is essentially an unsigned char, so 1 would return ' '.
  sql << "SELECT * FROM RiverLevels WHERE timestamp >= '" << startDate << "' LIMIT " << static_cast<int>(pageSize)
      << " OFFSET " << ((page - PAGE_OFFSET) * pageSize);
  LOG.debug_f("Executing SQL: %s", sql.str().c_str());
  const char* query = sql.str().c_str();
  cur_mem->execute(query);

  // Step 3: Fetch the columns
  column_names *columns = cur_mem->get_columns();

  row_values *row = nullptr;
  long head_count = 0;

  // Read the rows
  do {
    row = cur_mem->get_next_row();
    if (row != NULL) {
      char* endptr;
      double level = strtod(row->values[1], &endptr);
      if (endptr == row->values[1] || *endptr != '\0') {
        // Conversion failed
        LOG.error_f("Failed to convert string '%s' to double", row->values[1]);
        continue;
      }

      RiverReading reading{ .timestamp = row->values[0], .level = level };
    }
  } while (row != NULL);
  // Deleting the cursor also frees up memory used
  delete cur_mem;


  return result;
}
std::vector<RainfallReading> FloodRepository::getStationRainfallReadings(const char* stationName, const char* startDate,
                                                                         uint16_t page, uint8_t pageSize) const
{
  std::vector<RainfallReading> result;

  return result;
}
