//
// Created by Jamie Briggs on 10/07/2025.
//

#ifndef FLOODROUTES_H
#define FLOODROUTES_H

#include <WiFi.h>
#include <aWOT.h>

#include "IDisplay.h"


class FloodRoutes
{
  private:
  static constexpr uint8_t PORT = 80;

  static void logRequest(Request& request, Response& response);
  static void index(Request& request, Response& response);
  static void update(Request& request, Response& response);
  static void river(Request& request, Response& response);
  static void riverStation(Request& request, Response& response);
  WiFiServer m_server;
  Application m_app;
  static IDisplay* s_display;
  char m_expectHeader[20]{};

  public:
  FloodRoutes(IDisplay* printer);
  void loop();
};


#endif // FLOODROUTES_H
