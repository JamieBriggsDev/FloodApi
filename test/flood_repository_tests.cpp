//
// Created by Jamie Briggs on 16/07/2025.
//

// test/hardware_tests.cpp
#include <gtest/gtest.h>

#if defined(ARDUINO)
#include <Arduino.h>
#include <WiFi.h>

#include "db/FloodRepository.h"
#include "db/FloodRepository.h"
#include "db/FloodRepository.cpp"
#include "db/FloodSchema.h"

class FloorRepositoryTests : public ::testing::Test
{
protected:
  IFloodRepository* repository_ = nullptr;

  void SetUp() override
  {
    repository_ = new FloodRepository("/small_flood_downgraded.db");
    repository_->init();
  }
};

TEST_F(FloorRepositoryTests, GetAllStationNames)
{
  const auto stationMap = repository_->getAllStations();

  ASSERT_EQ(stationMap.size(), 11);
  // Verify each station name and ID mapping
  
  ASSERT_EQ(stationMap.at("hexham-firtrees"), "016140");
  ASSERT_TRUE(repository_->stationExists("hexham-firtrees"));
  ASSERT_EQ(stationMap.at("acomb-codlaw-hill"), "015313");
  ASSERT_TRUE(repository_->stationExists("acomb-codlaw-hill"));
  ASSERT_EQ(stationMap.at("catcleugh"), "010660");
  ASSERT_TRUE(repository_->stationExists("catcleugh"));
  ASSERT_EQ(stationMap.at("chirdon"), "010312");
  ASSERT_TRUE(repository_->stationExists("chirdon"));
  ASSERT_EQ(stationMap.at("kielder-ridge-end"), "008850");
  ASSERT_TRUE(repository_->stationExists("kielder-ridge-end"));
  ASSERT_EQ(stationMap.at("haltwhistle"), "014555");
  ASSERT_TRUE(repository_->stationExists("haltwhistle"));
  ASSERT_EQ(stationMap.at("knarsdale"), "013878");
  ASSERT_TRUE(repository_->stationExists("knarsdale"));
  ASSERT_EQ(stationMap.at("alston"), "013553");
  ASSERT_TRUE(repository_->stationExists("alston"));
  ASSERT_EQ(stationMap.at("hartside"), "013336");
  ASSERT_TRUE(repository_->stationExists("hartside"));
  ASSERT_EQ(stationMap.at("allenheads-allen-lodge"), "015347");
  ASSERT_TRUE(repository_->stationExists("allenheads-allen-lodge"));
  ASSERT_EQ(stationMap.at("garrigill-noonstones-hill"), "013045");
  ASSERT_TRUE(repository_->stationExists("garrigill-noonstones-hill"));
}

TEST_F(FloorRepositoryTests, GetRiversNoArguments)
{
  const auto riverReadings = repository_->getRiverReadings();

  ASSERT_EQ(riverReadings.size(), 12);
  const RiverReading expected{.timestamp = "2022-12-12 00:00:00", .level = 0.375};
  const RiverReading actual = riverReadings[0];
  ASSERT_EQ(actual, expected);

}



TEST_F(FloorRepositoryTests, GetSingleRiverLevel)
{
  const auto riverReadings = repository_->getRiverReadings("2022-12-12", 1, 1);

  ASSERT_EQ(riverReadings.size(), 1);
  const RiverReading expected{.timestamp = "2022-12-12 00:00:00", .level = 0.375};
  const RiverReading actual = riverReadings[0];
  ASSERT_EQ(actual, expected);

}

TEST_F(FloorRepositoryTests, GetThreeRiverLevels)
{
  const auto riverReadings = repository_->getRiverReadings("2020-01-25", 1, 3);

  ASSERT_EQ(riverReadings.size(), 3);
}



#endif