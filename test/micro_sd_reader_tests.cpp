//
// Created by Jamie Briggs on 16/07/2025.
//

// test/hardware_tests.cpp
#include <gtest/gtest.h>

// Disabled these tests as I don't MicroSDReader will be used as part of the FloodAPI.
#if false //defined(ARDUINO)
#include <Arduino.h>

#include "sd/MicroSDReader.cpp"
#include "sd/MicroSDReader.h"

class MicroSDReaderTests : public ::testing::Test
{
protected:
  MicroSDReader reader_;

  void SetUp() override
  {
    reader_.init();
  }
};

// Tests that require Arduino hardware
TEST_F(MicroSDReaderTests, TestListDirectory)
{
  auto file_entries = reader_.listDir(SD, "/");

  EXPECT_EQ(file_entries[0].name, "flood.db");
  EXPECT_EQ(file_entries[0].size, 25272320);
  EXPECT_EQ(file_entries[0].date, "2025-07-06 17:48:40");
  EXPECT_EQ(file_entries[0].isDirectory, false);
  EXPECT_EQ(file_entries[1].name, "testDocument.txt");
  EXPECT_EQ(file_entries[1].size, 6);
  EXPECT_EQ(file_entries[1].date, "2025-07-16 23:58:40");
  EXPECT_EQ(file_entries[1].isDirectory, false);
  EXPECT_EQ(file_entries[2].name, "testDirectory");
  EXPECT_EQ(file_entries[2].isDirectory, true);
}

TEST_F(MicroSDReaderTests, ReadTextFile)
{
  reader_.readFile(SD, "/testDocument.txt");
}


#endif