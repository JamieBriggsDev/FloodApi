//
// Created by Jamie Briggs on 17/07/2025.
//

#ifndef FLOODREPOSITORY_H
#define FLOODREPOSITORY_H


#include <vector>


#include "IFloodRepository.h"
#include "sqlite3.h"

// Forward declarations, avoids adding the header in this file
struct RiverReading;
struct RainfallReading;

class FloodRepository : public IFloodRepository
{
  sqlite3* m_floodDb;
  const char* m_dbPath;

  public:
  explicit FloodRepository(const char* dbPath) : m_floodDb(nullptr), m_dbPath(dbPath) {};
  ~FloodRepository() override
  {
    sqlite3_close(m_floodDb);
    m_floodDb = nullptr;
  };
  void init();
  std::vector<RiverReading> getRiverReadings(const char* startDate, uint16_t page,
                                             uint8_t pageSize) const override;
  std::vector<RainfallReading> getStationRainfallReadings(const char* stationName, const char* startDate,
                                                          uint16_t page, uint8_t pageSize) const override;
};


#endif // FLOODREPOSITORY_H
