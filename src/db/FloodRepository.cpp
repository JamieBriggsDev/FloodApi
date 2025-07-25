//
// Created by Jamie Briggs on 17/07/2025.
//

#include "../../src/db/FloodRepository.h"

#include <SD.h>
#include <SPI.h>
#include <sstream>
#include <string>

#include "FloodSchema.h"
#include "def_pin_outs.h"
#include "logger/def_logger_factory.h"

#define READ_ALL -1
#define PAGE_OFFSET 1

int openDb(const char* filename, sqlite3** db)
{
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

#if true
  LOG.debug("Beginning SPI");
  SPI.begin();
  LOG.debug("Beginning SD ");
  if (!SD.begin(MICRO_SD_CS_PIN))
  {
    LOG.error("Card Mount Failed");
    throw std::runtime_error("Card Mount Failed");
  }
#endif

#if true
  uint8_t cardType = SD.cardType();
  if (cardType == CARD_NONE)
  {
    LOG.error("No SD card attached");
    throw std::runtime_error("No SD card attached");
  }

#endif

#if true
  // Check the database file exists
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
#endif


#if true
  LOG.info("Initializing SQLite3...");
  int initialize = sqlite3_initialize();
  if (initialize != SQLITE_OK)
  {
    LOG.error_f("Failed to initialize SQLite3: %s", initialize);
    throw std::runtime_error("Failed to initialize SQLite3");
  }
#endif

#if true
  LOG.debug("Opening DB...");
  std::stringstream vfsPath;
  vfsPath << "/sd" << this->m_dbPath;
  if (openDb(vfsPath.str().c_str(), &m_floodDb) != SQLITE_OK)
  {
    LOG.error("Failed to open database");
    throw new std::runtime_error("Failed to open database");
  }
  LOG.info_f("Connected to database!");
#endif

  LOG.info("Completed initialization for FloodRepository");
}

std::map<std::string, std::string> FloodRepository::getAllStations()
{
  std::map<std::string, std::string> result;

  int rc = INT_MAX;
  sqlite3_stmt* stmt;
  const char* tail;

  std::string query = "SELECT * FROM StationNames";

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


std::vector<RiverReading> FloodRepository::getRiverReadings(const char* startDate, uint16_t page,
                                                            uint8_t pageSize) const
{
  std::vector<RiverReading> result;

  int rc = INT_MAX;
  sqlite3_stmt* stmt;
  const char* tail;
  int rowCount = 0;

  std::stringstream sql;
  // Casting pageSize to int as uint8_t is essentially an unsigned char, so 1 would return ' '.
  sql << "SELECT * FROM RiverLevels";
  if (startDate != nullptr && strlen(startDate) > 0)
  {
    sql << " WHERE timestamp >= '" << startDate << "'";
  }
  sql << " LIMIT " << static_cast<int>(pageSize) << " OFFSET " << ((page - PAGE_OFFSET) * pageSize);
  std::string query = sql.str();
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
    std::string timestamp(temp); // Creates a copy of the string data
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

  int rc = INT_MAX;
  sqlite3_stmt* stmt;
  const char* tail;
  int rowCount = 0;

  std::stringstream sql;
  // Casting pageSize to int as uint8_t is essentially an unsigned char, so 1 would return ' '.
  sql << "SELECT r.TimeStamp, sn.Name, r.Level FROM Rainfalls r";
  sql << " INNER JOIN StationNames sn ON r.StationId = sn.Id";
  if (startDate != nullptr && strlen(startDate) > 0)
  {
    sql << " WHERE timestamp >= '" << startDate << "'";
  }
  sql << " LIMIT " << static_cast<int>(pageSize) << " OFFSET " << ((page - PAGE_OFFSET) * pageSize);
  std::string query = sql.str();
  LOG.debug_f("Preparing query: %s", query.c_str());
  // Turn SQL statement into something SQLite can use. This will be the stmt object.
  rc = sqlite3_prepare_v2(m_floodDb, query.c_str(), READ_ALL, &stmt, &tail);
  if (rc != SQLITE_OK)
  {
    LOG.error_f("Failed to prepare statement: %s", sqlite3_errmsg(m_floodDb));
    throw new std::runtime_error("Failed to prepare statement");
  }

  LOG.debug("Stepping through statement");
  // Next, perform the step command. This will execute the prepared stmt object.
  while (sqlite3_step(stmt) == SQLITE_ROW)
  {
    // Map to struct and add to result.
    const char* temp = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
    std::string timestamp(temp); // Creates a copy of the string data
    const char* name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
    const double level = sqlite3_column_double(stmt, 2);
    RainfallReading reading{.timestamp = timestamp, .station = name, .level = level};
    result.push_back(reading);

    rowCount++;
  }

  LOG.debug_f("Finalizing. Found %d results.", result.size());
  // Finalize, which destroys the prepared statement and frees up resources for the next query.
  sqlite3_finalize(stmt);

  return result;
}
