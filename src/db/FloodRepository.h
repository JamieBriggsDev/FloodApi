//
// Created by Jamie Briggs on 17/07/2025.
//

#ifndef FLOODREPOSITORY_H
#define FLOODREPOSITORY_H

#include <cstdint>
#include <mutex>
#include <vector>


#include "IFloodRepository.h"
#include "sqlite3.h"

// Forward declarations, avoids adding the header in this file
struct RiverReading;
struct RainfallReading;
class FloodRepository : public IFloodRepository
{
  sqlite3* m_floodDb;
  mutable std::mutex m_mutex;  // mutable allows usage in const methods


  public:
  FloodRepository() : m_floodDb(nullptr) {};
  ~FloodRepository() override
  {
    sqlite3_close(m_floodDb);
    m_floodDb = nullptr;
  };
  void init();
  std::vector<RiverReading> getRiverReadings(const char* startDate, uint16_t page = 1,
                                             uint8_t pageSize = 12) const override;
  std::vector<RainfallReading> getStationRainfallReadings(const char* stationName, const char* startDate,
                                                          uint16_t page = 1, uint8_t pageSize = 12) const override;
};


#endif // FLOODREPOSITORY_H
