//
// Created by Jamie Briggs on 17/07/2025.
//

#ifndef IFLOODREPOSITORY_H
#define IFLOODREPOSITORY_H
#include <cstdint>
#include <vector>

#include "FloodSchema.h"

class IFloodRepository
{
  public:
  virtual ~IFloodRepository() = default;
  virtual void init() = 0;

  virtual bool stationExists(const char* stationName) const = 0;
  virtual std::vector<RiverReading> getRiverReadings(const char* startDate = "", uint16_t page = 1,
                                                     uint8_t pageSize = 12) const = 0;
  virtual std::vector<RainfallReading> getStationRainfallReadings(const char* stationName, const char* startDate = "",
                                                                  uint16_t page = 1, uint8_t pageSize = 12) const = 0;
};

#endif // IFLOODREPOSITORY_H
