//
// Created by Jamie Briggs on 16/07/2025.
//

// test/hardware_tests.cpp
#include <gtest/gtest.h>

#if defined(ARDUINO)
#include <Arduino.h>

#include "db/FloodRepository.h"
#include "db/FloodRepository.cpp"

class FloorRepositoryTests : public ::testing::Test
{
protected:
  FloodRepository repository_;

  void SetUp() override
  {
    repository_.init();
  }
};

TEST_F(FloorRepositoryTests, GetStationNames_Test)
{
  repository_.getAllStationNames();
}



#endif