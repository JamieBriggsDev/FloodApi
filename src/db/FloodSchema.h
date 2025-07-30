//
// Created by Jamie Briggs on 17/07/2025.
//


#ifndef SCHEMA_H
#define SCHEMA_H

#include <string>

namespace jbriggs::flood::db
{

  struct RiverReading
  {
    std::string timestamp;
    const double level;

    bool operator==(const RiverReading& other) const { return timestamp == other.timestamp && level == other.level; }
  };

  struct RainfallReading
  {
    std::string timestamp;
    std::string station;
    const double level;

    bool operator==(const RainfallReading& other) const
    {
      return timestamp == other.timestamp && station == other.station && level == other.level;
    }
  };

} // namespace jbriggs::flood::db
#endif // SCHEMA_H
