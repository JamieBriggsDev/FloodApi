//
// Created by Jamie Briggs on 23/07/2025.
//

#ifndef FLOODSDREPOSITORY_H
#define FLOODSDREPOSITORY_H
#include <FS.h>


#include "IFloodRepository.h"


class FloodSDRepository final : public IFloodRepository
{
  long findDatePosition(::File& file, const char* startDate) const;

  public:
  const size_t BUFFER_SIZE = 128; // Adjust based on your memory constraints
  static constexpr const char* RIVER_LEVELS_FILE = "/riverLevels.csv";
  static constexpr const char* RAINFALL_FILE = "/rainfalls.csv";
  static constexpr const char* STATION_NAMES = "/stationNames.csv";
  void init() override;
  std::vector<RiverReading> getRiverReadings(const char* startDate, uint16_t page, uint8_t pageSize) const override;
  std::vector<RainfallReading> getStationRainfallReadings(const char* stationName, const char* startDate, uint16_t page,
                                                          uint8_t pageSize) const override;
};


#endif // FLOODSDREPOSITORY_H