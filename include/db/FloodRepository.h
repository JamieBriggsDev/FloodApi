//
// Created by Jamie Briggs on 17/07/2025.
//

#ifndef FLOODREPOSITORY_H
#define FLOODREPOSITORY_H

#include "sqlite3.h"

class FloodRepository
{
  sqlite3* m_floodDb;

  public:
  FloodRepository() {};
  ~FloodRepository()
  {
    sqlite3_close(m_floodDb);
    m_floodDb = nullptr;
  };
  void init();
  void getAllStationNames();
};


#endif // FLOODREPOSITORY_H
