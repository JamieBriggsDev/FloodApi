//
// Created by Jamie Briggs on 17/07/2025.
//


#ifndef SCHEMA_H
#define SCHEMA_H

#include <string>

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

/*
 * Leaving this commented out as I don't think it's needed, but will save in case
enum Station
{
  ACOMB_CODLAW_HILL,
  ALLENHEADS_ALLEN_LODGE,
  ALSTON,
  CATCLEUGH,
  CHIRDON,
  GARRIGILL_NOONSTONES_HILL,
  HALTWHISTLE,
  HARTSIDE,
  HEXHAM_FIRTREES,
  KIELDER_RIDGE_END,
  KNARSDALE
};*/

#endif // SCHEMA_H
