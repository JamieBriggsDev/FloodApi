//
// Created by Jamie Briggs on 10/07/2025.
//

#include "FloodRoutes.h"

#include <Update.h>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>

#include "def_wifi_settings.h"
#include "display/IDisplay.h"
#include "logger/def_logger_factory.h"

// Add static member definition
void FloodRoutes::river()
{
  LOG.info("/river requested");

  auto start = std::chrono::system_clock::now();

  // TODO Re-enable this
  std::vector<RiverReading> river_readings;// = s_floodRepository->getRiverReadings("2025-12-25");

  // Convert to JSON
  const JsonDocument doc = s_floodMapper->getFloodData(river_readings);
  std::string json;
  serializeJsonPretty(doc, json);

  const char* result = json.c_str();

  auto end = std::chrono::system_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
  LOG.info_f("/river completed in %d milliseconds", duration.count());

  return m_server.send(200, "application/json", result);
}

void FloodRoutes::riverStation()
{
  return m_server.send(404, "application/json", "{}");
}

FloodRoutes::FloodRoutes(IDisplay* display, IFloodRepository* flood_repository, IFloodMapper* flood_mapper) :
    m_server(PORT)
{
  LOG.debug("Setting up FloodRoutes...");
  s_display = display;
  s_floodRepository = flood_repository;
  s_floodMapper = flood_mapper;

  // Setup routes
  LOG.debug("Setting up routes...");
  m_server.on("/river", HTTP_GET, [this]()
  {
    this->river();
  });
  m_server.on("/rainfall/{station}", HTTP_GET, [this]()
  {
    this->riverStation();
  });
  // Begin server
  LOG.debug("Starting server in FloodRoutes");
  m_server.begin();
}

void FloodRoutes::loop()
{
  m_server.handleClient();
}
