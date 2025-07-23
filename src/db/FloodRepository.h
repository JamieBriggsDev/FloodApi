//
// Created by Jamie Briggs on 17/07/2025.
//

#ifndef FLOODREPOSITORY_H
#define FLOODREPOSITORY_H

#include <vector>


#include "IFloodRepository.h"

// Forward declarations, avoids adding the header in this file
struct RiverReading;
struct RainfallReading;

class FloodRepository : public IFloodRepository
{



  public:
  void init() override;
  std::vector<RiverReading> getRiverReadings(const char* startDate, uint16_t page,
                                             uint8_t pageSize) const override;
  std::vector<RainfallReading> getStationRainfallReadings(const char* stationName, const char* startDate,
                                                          uint16_t page, uint8_t pageSize) const override;
};


#endif // FLOODREPOSITORY_H
