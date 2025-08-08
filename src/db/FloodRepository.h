//
// Created by Jamie Briggs on 17/07/2025.
//

#ifndef FLOODREPOSITORY_H
#define FLOODREPOSITORY_H


#include <map>
#include <vector>


#include "IFloodRepository.h"
#include "sqlite3.h"

// Forward declarations, avoids adding the header in this file
struct RiverReading;
struct RainfallReading;

namespace jbriggs::flood::db
{
  class FloodRepository final : public IFloodRepository
  {
    sqlite3* m_floodDb;
    const char* m_dbPath;
    std::map<std::string, std::string> m_stationMap;


public:
    explicit FloodRepository(const char* dbPath) : m_floodDb(nullptr), m_dbPath(dbPath) {};
    ~FloodRepository() override
    {
      sqlite3_close(m_floodDb);
      m_floodDb = nullptr;
    };
    void init() override;

    std::map<std::string, std::string> getAllStations() override;
    bool stationExists(std::string stationName) override
    {
      return m_stationMap.find(stationName) != m_stationMap.end();
    }
    std::vector<RiverReading> getRiverReadings(std::string startDate, int page, uint8_t pageSize) const override;
    std::vector<RainfallReading> getStationRainfallReadings(std::string stationName, std::string startDate,
                                                            int page, uint8_t pageSize) const override;
  };

} // namespace flood::db

#endif // FLOODREPOSITORY_H
