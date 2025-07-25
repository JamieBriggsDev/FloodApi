//
// Created by Jamie Briggs on 10/07/2025.
//

#include "FloodRoutes.h"

#include <ESPmDNS.h>
#include <Update.h>
#include <WebServer.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <uri/UriBraces.h>

#include "def_wifi_settings.h"
#include "display/IDisplay.h"
#include "logger/def_logger_factory.h"

String FloodRoutes::getQueryParameter(const String& param, const String& defaultValue)
{
  std::stringstream paramDisplay;
  paramDisplay << "Param " << param.c_str();
  LOG.debug_f("Param: %s, Default: %s", param, defaultValue.c_str());

  if (!m_server.hasArg(param))
  {
    LOG.debug_f("Param %s not found", param);
    displayParameterValue(paramDisplay.str().c_str(), defaultValue.length() == 0 ? "EMPTY" : defaultValue.c_str());
    return defaultValue.length() == 0 ? "" : defaultValue;
  }

  LOG.debug_f("Total request params: %d", m_server.args());
  LOG.debug_f("Getting param arg: %s", m_server.arg(param));
  s_display->displayText(paramDisplay.str().c_str(), m_server.arg(param).c_str(), FLASH);
  return m_server.arg(param);
}

// Add static member definition
void FloodRoutes::river()
{
  LOG.info("/river requested");

  s_display->displayText("Calling", "/river", FLASH);
  // Get request parameters
  // Get the date parameter
  String date = getQueryParameter("start");
  // Get limit parameter with default value
  int limit = std::stoi(getQueryParameter("page", "1").c_str());
  // Get page parameter with default value
  int pagesize = std::stoi(getQueryParameter("pagesize", "12").c_str());


  std::vector<RiverReading> river_readings = s_floodRepository->getRiverReadings(date.c_str(), limit, pagesize);

  // Convert to JSON
  const JsonDocument doc = s_floodMapper->getFloodData(river_readings);
  std::string json;
  serializeJsonPretty(doc, json);

  const char* result = json.c_str();


  return m_server.send(200, "application/json", result);
}

void FloodRoutes::rainfallStation(const String& stationName)
{
  String potFullPath = m_server.uri();
  // Get path param station name
  std::stringstream fullPath;
  fullPath << "/rainfall/" << stationName.c_str();
  LOG.info_f("/rainfall/{station} requested using %s", stationName);

  s_display->displayText("Calling", fullPath.str().c_str(), FLASH);

  // You can validate against your known stations
  std::string stationNameStr(stationName.c_str());
  if (!s_floodRepository->stationExists(stationNameStr))
  {
    m_server.send(404, "application/json", "{\"error\": \"Invalid station name. Station not found.\"}");
    return;
  }

  // Get request parameters
  // Get the date parameter
  String date = getQueryParameter("start", "2022-12-25");
  // Get limit parameter with default value
  int limit = std::stoi(getQueryParameter("page", "1").c_str());
  // Get page parameter with default value
  int pagesize = std::stoi(getQueryParameter("pagesize", "12").c_str());


  std::vector<RainfallReading> rainfall_readings =
      s_floodRepository->getStationRainfallReadings(stationName.c_str(), date.c_str(), limit, pagesize);

  // Convert to JSON
  const JsonDocument doc = s_floodMapper->getRainfallReadings(rainfall_readings);
  std::string json;
  serializeJsonPretty(doc, json);

  const char* result = json.c_str();

  return m_server.send(200, "application/json", result);
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
  // GET: /river
  m_server.on("/river", HTTP_GET,
              [this]()
              {
                auto start = std::chrono::system_clock::now();

                this->river();

                auto end = std::chrono::system_clock::now();
                auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
                LOG.info_f("/river completed in %d milliseconds", duration.count());
              });
  // GET: /rainfall/{stationName}
  m_server.on(UriBraces("/rainfall/{}"), HTTP_GET,
              [this]()
              {
                auto start = std::chrono::system_clock::now();
                this->rainfallStation(m_server.pathArg(0));

                auto end = std::chrono::system_clock::now();
                auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
                LOG.info_f("/river completed in %d milliseconds", duration.count());
              });

// Begin server
LOG.debug("Starting server in FloodRoutes");
m_server.begin();
}

void FloodRoutes::loop() { m_server.handleClient(); }
