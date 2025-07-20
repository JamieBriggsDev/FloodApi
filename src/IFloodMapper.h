//
// Created by Jamie Briggs on 20/07/2025.
//

#ifndef IFLOODMAPPER_H
#define IFLOODMAPPER_H

#include <ArduinoJson.h>
#include <vector>
#include "db/FloodSchema.h"

class IFloodMapper
{
public:
  virtual ~IFloodMapper() = default;
  virtual JsonDocument getFloodData(const std::vector<RiverReading>& riverReadings) const = 0;
  virtual JsonDocument getRainfallReadings(const std::vector<RainfallReading>& rainfallReadings) const = 0;
};

#endif //IFLOODMAPPER_H
