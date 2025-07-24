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