//
// Created by Jamie Briggs on 17/07/2025.
//

#ifndef IFLOODREPOSITORY_H
#define IFLOODREPOSITORY_H
#include <cstdint>
#include <map>
#include <vector>

#include "FloodSchema.h"


namespace jbriggs::flood::db
{
  class IFloodRepository
  {
public:
    virtual ~IFloodRepository() = default;
    virtual void init() = 0;

    virtual std::map<std::string, std::string> getAllStations() = 0;
    virtual bool stationExists(std::string stationName) = 0;
    virtual std::vector<RiverReading> getRiverReadings(std::string startDate = "", int page = 1,
                                                       uint8_t pageSize = 12) const = 0;
    virtual std::vector<RainfallReading> getStationRainfallReadings(std::string stationName, std::string startDate = "",
                                                                    int page = 1, uint8_t pageSize = 12) const = 0;
  };
} // namespace jbriggs::flood::db

#endif // IFLOODREPOSITORY_H
