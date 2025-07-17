//
// Created by Jamie Briggs on 16/07/2025.
//

#ifndef MICROSDREADER_H
#define MICROSDREADER_H

#include "FS.h"
#include "SD.h"
#include "SPI.h"

#include <vector>

struct FileEntry
{
  String name;
  uint32_t size;
  String date;
  bool isDirectory;
};

class MicroSDReader {
public:
  void init();
  std::vector<FileEntry> listDir(fs::FS &fs, const char * dirname, bool showHidden = false) const;
  void readFile(fs::FS &fs, const char * path) const;
};



#endif //MICROSDREADER_H
