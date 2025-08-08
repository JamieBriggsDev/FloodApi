//
// Created by Jamie Briggs on 17/07/2025.
//

#include "../../src/db/FloodRepository.h"
#include "../../src/db/FloodSchema.h"

#include <SD.h>
#include <SPI.h>
#include <sstream>
#include <string>

#include "../config/def_pin_outs.h"
#include "logger/def_logger_factory.h"

#define READ_ALL -1
#define PAGE_OFFSET 1

using namespace jbriggs::flood::db;

int openDb(const std::string& filename, sqlite3** db)
{
  const int rc = sqlite3_open(filename.c_str(), db);

  if (rc)
  {
    LOG.error_f("Can't open database: %s", sqlite3_errmsg(*db));
    return rc;
  }

  LOG.info_f("Opened database successfully: %s", filename.c_str());
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

  // Step 1: Initialize SPI and SD
  LOG.debug("Beginning SPI");
  SPI.begin();
  LOG.debug("Beginning SD ");
  while (!SD.begin(config::MICRO_SD_CS_PIN))
  {
    LOG.error("Card Mount Failed");
  }

  uint8_t cardType = SD.cardType();
  if (cardType == CARD_NONE)
  {
    LOG.error("No SD card attached");
    throw std::runtime_error("No SD card attached");
  }

  // Step 2: Check the database file exists
  if (SD.exists(this->m_dbPath))
  {
    LOG.debug_f("Database file '%s' exists on SD card", this->m_dbPath);
    File file = SD.open(this->m_dbPath);
    LOG.debug_f("File size: %d", file.size());
    file.close();
  }
  else
  {
    LOG.error("Database file not found on SD card");
  }

  // Step 3: Initialize SQLite3
  LOG.info("Initializing SQLite3...");
  int initialize = sqlite3_initialize();
  if (initialize != SQLITE_OK)
  {
    LOG.error_f("Failed to initialize SQLite3: %s", initialize);
    throw std::runtime_error("Failed to initialize SQLite3");
  }

  // Step 4: Open database file
  LOG.debug("Opening DB...");
  std::stringstream vfsPath;
  vfsPath << "/sd" << this->m_dbPath;
  if (openDb(vfsPath.str(), &m_floodDb) != SQLITE_OK)
  {
    LOG.error("Failed to open database");
    throw std::runtime_error("Failed to open database");
  }
  LOG.info_f("Connected to database!");

  // Step 5: Cache station names
  LOG.debug("Caching station names");
  auto stationNames = this->getAllStations();
  if (stationNames.empty())
  {
    LOG.error("Failed to get station names");
    throw std::runtime_error("Failed to get station names");
  }

  LOG.info("Completed initialization for FloodRepository");
}

std::map<std::string, std::string> FloodRepository::getAllStations()
{
  std::map<std::string, std::string> result;

  int rc = INT_MAX;
  sqlite3_stmt* stmt;

  const std::string query = "SELECT * FROM StationNames";

  LOG.debug_f("Preparing query: %s", query.c_str());
  // Turn SQL statement into something SQLite can use. This will be the stmt object.
  rc = sqlite3_prepare_v2(m_floodDb, query.c_str(), READ_ALL, &stmt, nullptr);
  if (rc != SQLITE_OK)
  {
    LOG.error_f("Failed to prepare statement: %s", sqlite3_errmsg(m_floodDb));
    throw std::runtime_error("Failed to prepare statement");
  }

  LOG.debug("Stepping through statement");
  // Next, perform the step command. This will execute the prepared stmt object.
  while (sqlite3_step(stmt) == SQLITE_ROW)
  {
    // Map to struct and add to result.
    std::string stationId = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
    std::string stationName = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
    result.insert({stationName, stationId});
  }

  LOG.debug_f("Finalizing. Found %d results.", result.size());
  // Finalize, which destroys the prepared statement and frees up resources for the next query.
  sqlite3_finalize(stmt);

  // Store station names
  this->m_stationMap = result;

  return result;
}


std::vector<jbriggs::flood::db::RiverReading> FloodRepository::getRiverReadings(std::string startDate, int page,
                                                            uint8_t pageSize) const
{
  std::vector<RiverReading> result;

  int rc = INT_MAX;
  sqlite3_stmt* stmt;

  std::stringstream sql;
  // Casting pageSize to int as uint8_t is essentially an unsigned char, so 1 would return ' '.
  sql << "SELECT REPLACE(r.TimeStamp, ' ', 'T') || 'Z' AS TimeStamp, r.Level FROM RiverLevels r";
  if (!startDate.empty())
  {
    sql << " WHERE timestamp >= '" << startDate << "'";
  }
  sql << " LIMIT " << static_cast<int>(pageSize) << " OFFSET " << ((page - PAGE_OFFSET) * pageSize);
  std::string query = sql.str();
  LOG.debug_f("Preparing query: %s", query.c_str());
  // Turn SQL statement into something SQLite can use. This will be the stmt object.
  rc = sqlite3_prepare_v2(m_floodDb, query.c_str(), READ_ALL, &stmt, nullptr);
  if (rc != SQLITE_OK)
  {
    LOG.error_f("Failed to prepare statement: %s", sqlite3_errmsg(m_floodDb));
    throw std::runtime_error("Failed to prepare statement");
  }

  LOG.debug("Stepping through statement");

  // Next, perform the step command. This will execute the prepared stmt object.
  while (sqlite3_step(stmt) == SQLITE_ROW)
  {
    // Map to struct and add to result.
    auto temp = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
    std::string timestamp(temp); // Creates a copy of the string data

    const double level = sqlite3_column_double(stmt, 1);
    RiverReading reading{.timestamp = timestamp, .level = level};
    result.push_back(reading);
  }

  LOG.debug_f("Finalizing. Found %d results.", result.size());
  // Finalize, which destroys the prepared statement and frees up resources for the next query.
  sqlite3_finalize(stmt);


  return result;
}
std::vector<jbriggs::flood::db::RainfallReading> FloodRepository::getStationRainfallReadings(std::string stationName, std::string startDate,
                                                                         int page, uint8_t pageSize) const
{
  std::vector<RainfallReading> result;
  LOG.debug_f("Finding rainfall readings for station: %s", stationName.c_str());
  std::string stationId = m_stationMap.at(stationName);
  int rc = INT_MAX;
  sqlite3_stmt* stmt;

  LOG.debug("Creating SQL statement");
  std::stringstream sql;
  // Casting pageSize to int as uint8_t is essentially an unsigned char, so 1 would return ' '.
  sql << "SELECT REPLACE(r.TimeStamp, ' ', 'T') || 'Z' AS TimeStamp, r.Level FROM Rainfalls r";
  sql << " WHERE r.StationId = '" << stationId << "'";
  if (!startDate.empty())
  {
    sql << " AND r.TimeStamp >= '" << startDate << "'";
  }
  sql << " LIMIT " << static_cast<int>(pageSize)
      << " OFFSET " << ((page - PAGE_OFFSET) * pageSize);
  std::string query = sql.str();
  LOG.debug_f("Preparing query: %s", query.c_str());
  // Turn SQL statement into something SQLite can use. This will be the stmt object.
  rc = sqlite3_prepare_v2(m_floodDb, query.c_str(), READ_ALL, &stmt, nullptr);
  if (rc != SQLITE_OK)
  {
    LOG.error_f("Failed to prepare statement: %s", sqlite3_errmsg(m_floodDb));
    throw std::runtime_error("Failed to prepare statement");
  }

  LOG.debug("Stepping through statement");
  // Next, perform the step command. This will execute the prepared stmt object.
  while (sqlite3_step(stmt) == SQLITE_ROW)
  {
    // Map to struct and add to result.
    auto temp = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
    std::string timestamp(temp); // Creates a copy of the string data
    LOG.debug_f("Found timestamp: %s", timestamp.c_str());
    const double level = sqlite3_column_double(stmt, 1);
    RainfallReading reading{.timestamp = timestamp, .station = stationName, .level = level};
    result.push_back(reading);
  }

  LOG.debug_f("Finalizing. Found %d results.", result.size());
  // Finalize, which destroys the prepared statement and frees up resources for the next query.
  sqlite3_finalize(stmt);

  return result;
}
