//
// Created by Jamie Briggs on 16/07/2025.
//

#include "def_pin_outs.h"

#include <time.h>
#include "logger/def_logger_factory.h"
#include "sd/MicroSDReader.h"

void MicroSDReader::init()
{
  if (!SD.begin(MICRO_SD_CS_PIN))
  {
    Serial.println("Card Mount Failed");
    throw std::runtime_error("Card Mount Failed");
  }

  uint8_t cardType = SD.cardType();

  if (cardType == CARD_NONE)
  {
    Serial.println("No SD card attached");
    throw std::runtime_error("No SD card attached");
  }

  Serial.print("SD Card Type: ");
  if (cardType == CARD_MMC)
  {
    Serial.println("MMC");
  }
  else if (cardType == CARD_SD)
  {
    Serial.println("SDSC");
  }
  else if (cardType == CARD_SDHC)
  {
    Serial.println("SDHC");
  }
  else
  {
    Serial.println("UNKNOWN");
  }

  uint64_t cardSize = SD.cardSize() / (1024 * 1024);
  Serial.printf("SD Card Size: %lluMB\n", cardSize);
}

std::vector<FileEntry> MicroSDReader::listDir(fs::FS& fs, const char* dirname, bool showHidden) const
{
  // TODO: Update when can do String format
  LOG.debug("Listing directory + dirname");

  std::vector<FileEntry> files;
  Serial.printf("Listing directory: %s\n", dirname);
  File root = fs.open(dirname);
  if (!root)
  {
    LOG.error("Failed to open directory");
    throw new std::runtime_error("Failed to open directory");
  }
  if (!root.isDirectory())
  {
    LOG.warning("Not a directory");
    return files;
  }

  File file = root.openNextFile();
  while (file)
  {
    if (!showHidden && file.name()[0] == '.')
    {
      file = root.openNextFile();
      continue;
    }
    if (file.isDirectory())
    {
      Serial.print("  DIR : ");
      Serial.println(file.name());
      FileEntry entry{.name = file.name(), .isDirectory = file.isDirectory()};
      files.push_back(entry);
    }
    else
    {
      Serial.print("  FILE: ");
      Serial.print(file.name());
      Serial.print("  SIZE: ");
      Serial.print(file.size());
      time_t rawTime = file.getLastWrite();
      char dateBuffer[30];
      tm* timeInfo = localtime(&rawTime);
      strftime(dateBuffer, sizeof(dateBuffer), "%Y-%m-%d %H:%M:%S", timeInfo);
      Serial.print("  DATE: ");
      Serial.println(dateBuffer);
      FileEntry entry{
          .name = file.name(), .size = file.size(), .date = String(dateBuffer), .isDirectory = file.isDirectory()};
      files.push_back(entry);
    }

    file = root.openNextFile();
  }
  return files;
}
void MicroSDReader::readFile(fs::FS& fs, const char* path) const
{
  // TODO: Update when can do String format
  LOG.debug("Reading file + path");

  File file = fs.open(path);
  if(!file){
    Serial.println("Failed to open file for reading");
    return;
  }

  Serial.print("Read from file: ");
  while(file.available()){
    Serial.write(file.read());
  }
  file.close();
}

/**
 *
 *https://github.com/siara-cc/esp32_arduino_sqlite3_lib
*#include <SPI.h>
#include <SD.h>
#include <sqlite3.h>

#define SD_CS 5

sqlite3 *db;
sqlite3_stmt *res;

void setup() {
  Serial.begin(115200);
  if (!SD.begin(SD_CS)) {
    Serial.println("SD card mount failed!");
    return;
  }

  if (sqlite3_open("/sd/mydb.sqlite", &db)) {
    Serial.printf("Can't open database: %s\n", sqlite3_errmsg(db));
    return;
  }

  const char *sql = "SELECT * FROM my_table;";
  if (sqlite3_prepare_v2(db, sql, -1, &res, NULL) == SQLITE_OK) {
    while (sqlite3_step(res) == SQLITE_ROW) {
      Serial.printf("Row: %s\n", sqlite3_column_text(res, 0));
    }
    sqlite3_finalize(res);
  } else {
    Serial.println("Failed to execute query");
  }

  sqlite3_close(db);
}

void loop() {
  // Nothing here
}
 */
