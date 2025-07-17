//
// Created by Jamie Briggs on 17/07/2025.
//

#include "../../src/db/FloodRepository.h"

#include <SD.h>
#include <SD_MMC.h>
#include <SPI.h>
#include <mutex>
#include <sstream>
#include <string>

#include "FloodSchema.h"
#include "def_pin_outs.h"
#include "logger/def_logger_factory.h"

#define READ_ALL -1
#define PAGE_OFFSET 1

int openDb(const char* filename, sqlite3** db)
{
  LOG.info_f("Attempting to open database at: %s", filename);

  const int rc = sqlite3_open(filename, db);

  if (rc)
  {
    LOG.error_f("Can't open database: %s", sqlite3_errmsg(*db));
    return rc;
  }

  LOG.info_f("Opened database successfully: %s", filename);
  return rc;
}

const char* data = "Callback function called";
static int callback(void* data, int argc, char** argv, char** azColName)
{
  LOG.info_f("%s: ", (const char*)data);
  for (int i = 0; i < argc; i++)
  {
    LOG.info_f("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
  }
  LOG.info("\n");
  return 0;
}

char* zErrMsg = nullptr;
int db_exec(sqlite3* db, const char* sql)
{
  Serial.println(sql);
  long start = micros();
  int rc = sqlite3_exec(db, sql, callback, (void*)data, &zErrMsg);
  if (rc != SQLITE_OK)
  {
    LOG.error_f("SQL Error: %s", zErrMsg);
    sqlite3_free(zErrMsg);
  }
  else
  {
    LOG.info_f("Operation done successfully: %s", sql);
  }
  LOG.info_f("Time taken: %d", micros() - start);
  return rc;
}


void FloodRepository::init()
{
  LOG.info("Initializing FloodRepository");

  LOG.debug("Beginning SPI");
  SPI.begin();
  LOG.debug("Beginning SD ");
  if (!SD.begin(MICRO_SD_CS_PIN))
  {
    LOG.error("Card Mount Failed");
    throw std::runtime_error("Card Mount Failed");
  }

  // Check the database file exists
  if (SD.exists("/flood_downgraded.db"))
  {
    LOG.debug("Database file exists on SD card");
    File file = SD.open("/flood_downgraded.db");
    LOG.debug_f("File size: %d", file.size());
    file.close();
  }
  else
  {
    LOG.error("Database file not found on SD card");
  }

  uint8_t cardType = SD.cardType();
  if (cardType == CARD_NONE)
  {
    LOG.error("No SD card attached");
    throw std::runtime_error("No SD card attached");
  }

  LOG.info("Initializing SQLite3...");
  int initialize = sqlite3_initialize();
  if (initialize != SQLITE_OK)
  {
    LOG.error_f("Failed to initialize SQLite3: %s", initialize);
    throw std::runtime_error("Failed to initialize SQLite3");
  }

  LOG.debug("Opening DB...");
  if (openDb("/sd/flood_downgraded.db", &m_floodDb) != SQLITE_OK)
  {
    LOG.error("Failed to open database");
    throw new std::runtime_error("Failed to open database");
  }

  LOG.info("Completed initialization for FloodRepository");
}


std::vector<RiverReading> FloodRepository::getRiverReadings(const char* startDate, uint16_t page, uint8_t pageSize) const
{
  std::lock_guard<std::mutex> lock(m_mutex);
  std::vector<RiverReading> result;

  std::stringstream sql;
  // Casting pageSize to int as uint8_t is essentially an unsigned char, so 1 would return ' '.
  sql << "SELECT * FROM RiverLevels WHERE timestamp >= '" << startDate << "' LIMIT " << static_cast<int>(pageSize)
      << " OFFSET " << ((page - PAGE_OFFSET) * pageSize);
  std::string query = sql.str();

  int rc = INT_MAX;
  sqlite3_stmt* stmt;
  const char* tail;
  int rowCount = 0;

  /**
   * Think of it like this:
   * `prepare`: Compiling a recipe
   * `step`: Following each step of the recipe
   * `finalize`: Cleaning up the kitchen when you're done
   */

  LOG.debug_f("Preparing query: %s", query.c_str());
  // Turn SQL statement into something SQLite can use. This will be the stmt object.
  rc = sqlite3_prepare_v2(m_floodDb, query.c_str(), READ_ALL, &stmt, &tail);
  if (rc != SQLITE_OK)
  {
    LOG.error_f("Failed to prepare statement: %s", sqlite3_errmsg(m_floodDb));
    throw new std::runtime_error("Failed to prepare statement");
  }

  LOG.debug("Stepping through statement");
  // TODO: This will run out of memory if unbound, keep an eye on it.
  //  It may be that I have to push the data to browser and flush it earlier.
  // Next, perform the step command. This will execute the prepared stmt object.
  while (sqlite3_step(stmt) == SQLITE_ROW)
  {
    // Map to struct and add to result.
    const char* temp = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
    std::string timestamp(temp);  // Creates a copy of the string data
    const double level = sqlite3_column_double(stmt, 1);
    RiverReading reading{.timestamp = timestamp, .level = level};
    result.push_back(reading);

    rowCount++;
  }

  LOG.debug_f("Finalizing. Found %d results.", result.size());
  // Finalize, which destroys the prepared statement and frees up resources for the next query.
  sqlite3_finalize(stmt);


  return result;
}
std::vector<RainfallReading> FloodRepository::getStationRainfallReadings(const char* stationName, const char* startDate,
                                                                         uint16_t page, uint8_t pageSize) const
{
  std::vector<RainfallReading> result;

  return result;
}
