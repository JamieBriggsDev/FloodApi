//
// Created by Jamie Briggs on 10/07/2025.
//

#ifndef FLOODROUTES_H
#define FLOODROUTES_H

#include <WiFi.h>
#include <aWOT.h>

#include "IFloodMapper.h"
#include "db/IFloodRepository.h"
#include "display/IDisplay.h"

class FloodRoutes
{
  private:
  static constexpr uint8_t PORT = 80;

  static void logRequest(Request& request, Response& response);
  static void river(Request& request, Response& response);
  static void riverStation(Request& request, Response& response);
  WiFiServer m_server;
  Application m_app;
  static IFloodRepository* s_floodRepository;
  static IDisplay* s_display;
  static IFloodMapper* s_floodMapper;
  char m_expectHeader[20]{};

  public:
  FloodRoutes(IDisplay* display, IFloodRepository* flood_repository, IFloodMapper* flood_mapper);
  void loop();
};


#endif // FLOODROUTES_H
