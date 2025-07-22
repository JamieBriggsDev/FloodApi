//
// Created by Jamie Briggs on 16/07/2025.
//

// test/hardware_tests.cpp
#include <gtest/gtest.h>

#if defined(ARDUINO)
#include <Arduino.h>
#include <WiFi.h>

#include "db/FloodRepository.cpp"
#include "db/FloodRepository.h"
#include "db/FloodSchema.h"
#include "def_wifi_settings.cpp"
#include "def_wifi_settings.h"
#include "MySQL_Packet.h"

class FloorRepositoryTests : public ::testing::Test
{
protected:
  FloodRepository repository_;

  void SetUp() override
  {
    LOG.debug_f("Connecting to WiFi: %s", WIFI_SSID);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    while (WiFiClass::status() != WL_CONNECTED)
    {
      delay(500);
      Serial.print("Connecting..");
    }
    LOG.debug("Connected to WiFi!");

    repository_.init();
  }
};


TEST_F(FloorRepositoryTests, GetSingleRiverLevel)
{
  const auto riverReadings = repository_.getRiverReadings("2022-12-12", 1, 1);

  ASSERT_EQ(riverReadings.size(), 1);
  const RiverReading expected{.timestamp = "2022-12-12 00:00:00", .level = 0.375};
  const RiverReading actual = riverReadings[0];
  ASSERT_EQ(actual, expected);

}

TEST_F(FloorRepositoryTests, GetThreeRiverLevels)
{
  const auto riverReadings = repository_.getRiverReadings("2020-01-25", 1, 3);

  ASSERT_EQ(riverReadings.size(), 3);
}



#endif