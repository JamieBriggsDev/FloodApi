//
// Created by Jamie Briggs on 10/07/2025.
//

#ifndef FLOODROUTES_H
#define FLOODROUTES_H

#include <WebServer.h>
#include <string>

#include "IFloodMapper.h"
#include "db/IFloodRepository.h"
#include "display/IDisplay.h"

class FloodRoutes
{
  void river();
  void rainfallStation(const std::string& stationName);
  WebServer m_server;
  IFloodRepository* s_floodRepository;
  IDisplay* s_display;
  IFloodMapper* s_floodMapper;
  char m_expectHeader[20]{};

  std::string getQueryParameter(const std::string& param, const std::string& defaultValue = "");
  void displayParameterValue(const std::string& paramName, const std::string& value) const
  {
    s_display->displayText(paramName, value, FLASH);
  }

  public:
  FloodRoutes(IDisplay* display, IFloodRepository* flood_repository, IFloodMapper* flood_mapper);
  void loop();
};


#endif // FLOODROUTES_H
