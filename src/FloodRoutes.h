//
// Created by Jamie Briggs on 10/07/2025.
//

#ifndef FLOODROUTES_H
#define FLOODROUTES_H

#include <WebServer.h>
#include <string>

#include "db/IFloodRepository.h"
#include "display/IDisplay.h"
#include "mapper/IFloodMapper.h"

using namespace jbriggs;
using namespace jbriggs::flood;

class FloodRoutes
{
  void river();
  void rainfallStation(const std::string& stationName);
  WebServer m_server;
  db::IFloodRepository* s_floodRepository;
  common::display::IDisplay* s_display;
  mapper::IFloodMapper* s_floodMapper;
  char m_expectHeader[20]{};

  std::string getQueryParameter(const std::string& param, const std::string& defaultValue = "");
  void displayParameterValue(const std::string& paramName, const std::string& value) const
  {
    s_display->displayText(paramName, value, common::display::FLASH);
  }

  public:
  FloodRoutes(common::display::IDisplay* display, db::IFloodRepository* flood_repository,
                  mapper::IFloodMapper* flood_mapper);
  void loop();
};


#endif // FLOODROUTES_H
