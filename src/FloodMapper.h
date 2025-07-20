//
// Created by Jamie Briggs on 20/07/2025.
//

#ifndef FLOODMAPPER_H
#define FLOODMAPPER_H

#include <ArduinoJson.h>
#include <vector>
#include "db/FloodSchema.h"

class FloodMapper
{
  public:
  static JsonDocument getFloodData(const std::vector<RiverReading>& riverReadings);
  static JsonDocument getRainfallReadings(const std::vector<RainfallReading>& rainfallReadings);
};


#endif // FLOODMAPPER_H
