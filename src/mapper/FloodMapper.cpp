//
// Created by Jamie Briggs on 20/07/2025.
//

#include "FloodMapper.h"

#include "../logger/def_logger_factory.h"

JsonDocument FloodMapper::getFloodData(const std::vector<RiverReading>& riverReadings) const
{
  JsonDocument doc;
  for (const auto& riverReading : riverReadings)
  {
    JsonDocument reading;
    reading["timestamp"] = riverReading.timestamp;
    reading["level"] = riverReading.level;
    if (!doc["readings"].add(reading))
    {
      LOG.error("Failed to add reading");
      throw std::runtime_error("Failed to add reading");
    }
  }

  std::string json;
  serializeJsonPretty(doc, json);
  LOG.debug(json.c_str());

  return doc;
}
JsonDocument FloodMapper::getRainfallReadings(const std::vector<RainfallReading>& rainfallReadings) const
{
  JsonDocument doc;
  for (const auto& rainfallReading : rainfallReadings)
  {
    JsonDocument reading;
    reading["timestamp"] = rainfallReading.timestamp;
    reading["level"] = rainfallReading.level;
    reading["station"] = rainfallReading.station;
    if (!doc["readings"].add(reading))
    {
      LOG.error("Failed to add reading");
      throw std::runtime_error("Failed to add reading");
    }
  }

  std::string json;
  serializeJsonPretty(doc, json);
  LOG.debug(json.c_str());

  return doc;
}
