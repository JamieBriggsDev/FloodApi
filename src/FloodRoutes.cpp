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
#include <iomanip>
#include <uri/UriBraces.h>

#include "def_wifi_settings.h"
#include "display/IDisplay.h"
#include "logger/def_logger_factory.h"

std::string FloodRoutes::getQueryParameter(const std::string& param, const std::string& defaultValue)
{
  std::stringstream paramDisplay;
  paramDisplay << "Param " << param;
  LOG.debug_f("Param: %s, Default: %s", param.c_str(), defaultValue.c_str());

  const String paramName(param.c_str());
  if (!m_server.hasArg(paramName))
  {
    LOG.debug_f("Param %s not found", param.c_str());
    displayParameterValue(paramDisplay.str(), defaultValue.empty() ? "EMPTY" : defaultValue);
    return defaultValue.empty() ? "" : defaultValue;
  }

  LOG.debug_f("Total request params: %d", m_server.args());
  const auto paramValue = m_server.arg(paramName);
  std::string result(paramValue.c_str(), paramValue.length());
  LOG.debug_f("Getting param arg: %s", result);
  s_display->displayText(paramDisplay.str(), result, common::display::FLASH);
  return result;
}

// Add static member definition
void FloodRoutes::river()
{
  LOG.info("/river requested");

  s_display->displayText("Calling", "/river", common::display::FLASH);
  // Get request parameters
  // Get the date parameter
  const std::string date = getQueryParameter("start");
  // Get limit parameter with default value
  const int limit = std::stoi(getQueryParameter("page", "1"));
  // Get page parameter with default value
  const int pagesize = std::stoi(getQueryParameter("pagesize", "12"));


  const std::vector<db::RiverReading> readings = s_floodRepository->getRiverReadings(date, limit, pagesize);

  // Convert to JSON
  const JsonDocument doc = s_floodMapper->getFloodData(readings);
  std::string json;
  serializeJsonPretty(doc, json);

  const char* result = json.c_str();


  return m_server.send(200, "application/json", result);
}

void FloodRoutes::rainfallStation(const std::string& stationName)
{
  // Get path param station name
  std::stringstream fullPath;
  fullPath << "/rainfall/" << stationName;
  LOG.info_f("/rainfall/{station} requested using %s", stationName);

  s_display->displayText("Calling", fullPath.str(), common::display::FLASH);

  // You can validate against your known stations
  if (!s_floodRepository->stationExists(stationName))
  {
    m_server.send(404, "application/json", R"({"error": "Invalid station name. Station not found."})");
    return;
  }

  // Get request parameters
  // Get the date parameter
  const std::string date = getQueryParameter("start", "2022-12-25");
  // Get limit parameter with default value
  const int limit = std::stoi(getQueryParameter("page", "1"));
  // Get page parameter with default value
  const int pagesize = std::stoi(getQueryParameter("pagesize", "12"));


  const std::vector<db::RainfallReading> rainfall_readings =
      s_floodRepository->getStationRainfallReadings(stationName, date, limit, pagesize);

  // Convert to JSON
  const JsonDocument doc = s_floodMapper->getRainfallReadings(rainfall_readings);
  std::string json;
  serializeJsonPretty(doc, json);

  return m_server.send(200, "application/json", json.c_str());
}


FloodRoutes::FloodRoutes(common::display::IDisplay* display, db::IFloodRepository* flood_repository, IFloodMapper* flood_mapper) :
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
                const auto start = std::chrono::system_clock::now();

                this->river();

                const auto end = std::chrono::system_clock::now();
                const auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
                LOG.info_f("/river completed in %d milliseconds", duration.count());
              });
  // GET: /rainfall/{stationName}
  m_server.on(UriBraces("/rainfall/{}"), HTTP_GET,
              [this]()
              {
                const auto start = std::chrono::system_clock::now();
                const auto pathArg = m_server.pathArg(0);
                const std::string stationName(pathArg.c_str(), pathArg.length());
                this->rainfallStation(stationName);

                const auto end = std::chrono::system_clock::now();
                const auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
                LOG.info_f("/river completed in %d milliseconds", duration.count());
              });

// Begin server
LOG.debug("Starting server in FloodRoutes");
m_server.begin();
}

void FloodRoutes::loop() { m_server.handleClient(); }
