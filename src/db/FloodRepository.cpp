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

#include "FloodSchema.h"
#include "MySQL_Connection.h"
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

  if (conn.connect(DB_HOSTNAME, DB_PORT, DB_USERNAME, DB_PASSWORD, DB_NAME))
  {
    delay(1000);
    LOG.debug_f("Connecting to database: %s", DB_HOSTNAME);
  }

  LOG.info("Completed initialization for FloodRepository");
}


std::vector<RiverReading> FloodRepository::getRiverReadings(const char* startDate, uint16_t page, uint8_t pageSize) const
{
  std::vector<RiverReading> result;

  std::stringstream sql;
  // Casting pageSize to int as uint8_t is essentially an unsigned char, so 1 would return ' '.
  sql << "SELECT * FROM RiverLevels WHERE timestamp >= '" << startDate << "' LIMIT " << static_cast<int>(pageSize)
      << " OFFSET " << ((page - PAGE_OFFSET) * pageSize);
  std::string query = sql.str();


  return result;
}
std::vector<RainfallReading> FloodRepository::getStationRainfallReadings(const char* stationName, const char* startDate,
                                                                         uint16_t page, uint8_t pageSize) const
{
  std::vector<RainfallReading> result;

  return result;
}
