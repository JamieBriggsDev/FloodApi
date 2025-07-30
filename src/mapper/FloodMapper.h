//
// Created by Jamie Briggs on 20/07/2025.
//

#ifndef FLOODMAPPER_H
#define FLOODMAPPER_H

#include "IFloodMapper.h"

namespace jbriggs::flood::mapper
{

  class FloodMapper : public IFloodMapper
  {
public:
    JsonDocument getFloodData(const std::vector<db::RiverReading>& riverReadings) const override;
    JsonDocument getRainfallReadings(const std::vector<db::RainfallReading>& rainfallReadings) const override;
  };

} // namespace jbriggs::flood::mapper

#endif // FLOODMAPPER_H
