//
// Created by Jamie Briggs on 20/07/2025.
//

#ifndef FLOODMAPPER_H
#define FLOODMAPPER_H

#include "IFloodMapper.h"

class FloodMapper : public IFloodMapper
{
  public:
  JsonDocument getFloodData(const std::vector<RiverReading>& riverReadings) const override;
  JsonDocument getRainfallReadings(const std::vector<RainfallReading>& rainfallReadings) const override;
};


#endif // FLOODMAPPER_H
