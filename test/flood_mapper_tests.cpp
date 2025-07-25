//
// Created by Jamie Briggs on 20/07/2025.
//


#include <gtest/gtest.h>
#include <cstring>

#include "../src/mapper/FloodMapper.cpp"
#include "FloodMapper.h"
#include "db/FloodSchema.h"

class FloodMapperTests : public ::testing::Test
{
protected:
  FloodMapper mapper_;
};

TEST_F(FloodMapperTests, shouldMapOneRiver)
{
  // Given
  RiverReading reading{ "2022-12-12 00:00:00", 0.375 };
  std::vector<RiverReading> riverReadings{ reading };
  // When
  JsonDocument result = mapper_.getFloodData(riverReadings);
  // Then
  auto elementOne = result["readings"][0].as<JsonObject>();
  ASSERT_TRUE(strcmp(elementOne["timestamp"].as<const char*>(), "2022-12-12 00:00:00") == 0);
  ASSERT_EQ(elementOne["level"].as<double>(), 0.375);
}

TEST_F(FloodMapperTests, shouldMapTwoRivers)
{
  // Given
  RiverReading reading{ "2022-12-12 00:00:00", 0.375 };
  RiverReading readingTwo{"2023-01-15 10:12:13", 1.5 };
  std::vector<RiverReading> riverReadings{ reading, readingTwo };
  // When
  JsonDocument result = mapper_.getFloodData(riverReadings);
  // Then
  auto elementOne = result["readings"][0].as<JsonObject>();
  auto elementTwo = result["readings"][1].as<JsonObject>();
  ASSERT_TRUE(strcmp(elementOne["timestamp"].as<const char*>(), "2022-12-12 00:00:00") == 0);
  ASSERT_EQ(elementOne["level"].as<double>(), 0.375);
  ASSERT_TRUE(strcmp(elementTwo["timestamp"].as<const char*>(), "2023-01-15 10:12:13") == 0);
  ASSERT_EQ(elementTwo["level"].as<double>(), 1.5);
}

TEST_F(FloodMapperTests, shouldMapOneRainfallReading)
{
  // Given
  RainfallReading reading{ "2022-12-12 00:00:00", "station-one", 0.375 };
  std::vector<RainfallReading> rainfallReadings{ reading };
  // When
  JsonDocument result = mapper_.getRainfallReadings(rainfallReadings);
  // Then
  auto elementOne = result["readings"][0].as<JsonObject>();
  ASSERT_TRUE(strcmp(elementOne["timestamp"].as<const char*>(), "2022-12-12 00:00:00") == 0);
  ASSERT_TRUE(strcmp(elementOne["station"].as<const char*>(), "station-one") == 0);
  ASSERT_EQ(elementOne["level"].as<double>(), 0.375);
}

TEST_F(FloodMapperTests, shouldMapTwoRainfallReadings)
{
  // Given
  RainfallReading reading{ "2022-12-12 00:00:00", "station-one", 0.375 };
  RainfallReading readingTwo{"2023-01-15 10:12:13", "station-two", 1.5 };
  std::vector<RainfallReading> rainfallReadings{ reading, readingTwo};
  // When
  JsonDocument result = mapper_.getRainfallReadings(rainfallReadings);
  // Then
  auto elementOne = result["readings"][0].as<JsonObject>();
  auto elementTwo = result["readings"][1].as<JsonObject>();
  ASSERT_TRUE(strcmp(elementOne["timestamp"].as<const char*>(), "2022-12-12 00:00:00") == 0);
  ASSERT_TRUE(strcmp(elementOne["station"].as<const char*>(), "station-one") == 0);
  ASSERT_EQ(elementOne["level"].as<double>(), 0.375);
  ASSERT_TRUE(strcmp(elementTwo["timestamp"].as<const char*>(), "2023-01-15 10:12:13") == 0);
  ASSERT_TRUE(strcmp(elementTwo["station"].as<const char*>(), "station-two") == 0);
  ASSERT_EQ(elementTwo["level"].as<double>(), 1.5);
}