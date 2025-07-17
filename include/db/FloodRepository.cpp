//
// Created by Jamie Briggs on 17/07/2025.
//

#include "FloodRepository.h"

#include <SD.h>
#include <SD_MMC.h>
#include <SPI.h>

#include "def_pin_outs.h"
#include "logger/def_logger_factory.h"

int openDb(const char* filename, sqlite3** db)
{
  LOG.info_f("Attempting to open database at: %s", filename);

  sqlite3_config(SQLITE_CONFIG_SINGLETHREAD);

  //int rc = sqlite3_open_v2(filename, db, SQLITE_OPEN_READONLY | SQLITE_OPEN_FULLMUTEX,
  //                         "sd"); // Explicitly specify the VFS as "sd"
  int rc = sqlite3_open(filename, db); // Explicitly specify the VFS as "sd"

  if (rc)
  {
    LOG.error_f("Can't open database: %s", sqlite3_errmsg(*db));
    return rc;
  }

  // Configure database connection
  const char* pragmas[] = {"PRAGMA temp_store = MEMORY;", "PRAGMA journal_mode = OFF;",
                           "PRAGMA synchronous = OFF;",   "PRAGMA cache_size = 8000;",
                           "PRAGMA page_size = 4096;",    "PRAGMA locking_mode = EXCLUSIVE;"};

  char* errMsg = nullptr;
  for (const char* pragma : pragmas)
  {
    rc = sqlite3_exec(*db, pragma, nullptr, nullptr, &errMsg);
    if (rc != SQLITE_OK)
    {
      LOG.error_f("Failed to set pragma: %s", pragma);
      LOG.error(errMsg);
      sqlite3_free(errMsg);
      return rc;
    }
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

char* zErrMsg = 0;
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

void dumpFileHeader(const char* filename)
{
  File file = SD.open(filename);
  if (!file)
  {
    LOG.error("Failed to open file for header check");
    return;
  }

  uint8_t header[16];
  size_t bytesRead = file.read(header, sizeof(header));

  LOG.info("File header bytes:");
  String headerHex;
  for (size_t i = 0; i < bytesRead; i++)
  {
    if (header[i] < 16)
      headerHex += "0";
    headerHex += String(header[i], HEX);
    headerHex += " ";
  }
  LOG.info(headerHex.c_str());

  file.close();
}

bool verifyDatabaseStructure(const char* filename)
{
  File file = SD.open(filename);
  if (!file)
  {
    LOG.error("Failed to open database for verification");
    return false;
  }

  const size_t CHUNK_SIZE = 1024;
  uint8_t buffer[CHUNK_SIZE];
  size_t totalBytesRead = 0;
  size_t pageSize = 0;

  // Read and verify the first page
  size_t bytesRead = file.read(buffer, 100); // Read first 100 bytes
  if (bytesRead >= 100)
  {
    // Page size is stored at byte offset 16
    pageSize = (buffer[16] << 8) | buffer[17];
    LOG.info("SQLite page size: " + String(pageSize));

    // Number of pages is stored at byte offset 28
    uint32_t numPages = (buffer[28] << 24) | (buffer[29] << 16) | (buffer[30] << 8) | buffer[31];
    LOG.info("Number of pages: " + String(numPages));

    // Database size should be pageSize * numPages
    LOG.info("Calculated DB size: " + String(pageSize * numPages));
    LOG.info("Actual file size: " + String(file.size()));
  }

  file.close();
  return true;
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

  // Add these debug lines
  if (SD.exists("/flood_downgraded.db"))
  {
    LOG.debug("Database file exists on SD card");
    File file = SD.open("/flood_downgraded.db");
    LOG.debug("File size: ");
    LOG.debug(String(file.size()));
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

  LOG.debug("Checking database file header...");
  dumpFileHeader("/flood_downgraded.db");

  LOG.debug("Verifying database structure...");
  verifyDatabaseStructure("/flood_downgraded.db");

  LOG.debug("Opening DB...");
  if (openDb("/sd/flood_downgraded.db", &m_floodDb) != SQLITE_OK) {
    LOG.error("Failed to open database");
    throw new std::runtime_error("Failed to open database");
  }

  LOG.info("Completed initialization for FloodRepository");
}

void FloodRepository::getAllStationNames()
{
  LOG.info("Getting all station names");
  const char *sql = "select * from RiverLevels LIMIT 10;";
  int rc = INT_MAX;
  rc = db_exec(m_floodDb, sql);
  if (rc != SQLITE_OK) {
    sqlite3_close(m_floodDb);
    return;
  }

  sqlite3_close(m_floodDb);
  m_floodDb = nullptr;
}
