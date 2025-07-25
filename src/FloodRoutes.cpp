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

#include "def_wifi_settings.h"
#include "display/IDisplay.h"
#include "logger/def_logger_factory.h"

std::string FloodRoutes::getQueryParameter(const char* param, const std::string& defaultValue)
{
  std::stringstream paramDisplay;
  paramDisplay << "PRM: " << param;


  if (!m_server.hasArg(param))
  {
    displayParameterValue(paramDisplay.str().c_str(), defaultValue.empty() ? "EMPTY" : defaultValue.c_str());
    return defaultValue.empty() ? "" : defaultValue;
  }

  const auto value = m_server.arg(param).c_str();
  s_display->displayText(paramDisplay.str().c_str(), value, FLASH);
  std::string result(value);
  return result;
}

// Add static member definition
void FloodRoutes::river()
{
  LOG.info("/river requested");

  s_display->displayText("Calling", "/river", FLASH);
  // Get request parameters
  // Get the date parameter
  std::string date = getQueryParameter("start");
  // Get limit parameter with default value
  int limit = std::stoi(getQueryParameter("page", "1"));
  // Get page parameter with default value
  int pagesize = std::stoi(getQueryParameter("pagesize", "12"));


  std::vector<RiverReading> river_readings = s_floodRepository->getRiverReadings(date.c_str(), limit, pagesize);

  // Convert to JSON
  const JsonDocument doc = s_floodMapper->getFloodData(river_readings);
  std::string json;
  serializeJsonPretty(doc, json);

  const char* result = json.c_str();


  return m_server.send(200, "application/json", result);
}

void FloodRoutes::riverStation()
{
  LOG.info("/rainfall/{station} requested");

  // Get path param station name
  std::string stationName = std::string(m_server.uri().substring(strlen("/rainfall/")).c_str());

  s_display->displayText("Calling", ("/river" + stationName).c_str(), FLASH);


  // You can validate against your known stations
  if (!s_floodRepository->stationExists(stationName.c_str()))
  {
    m_server.send(404, "application/json", "{\"error\": \"Invalid station name. Station not found.\"}");
    return;
  }


  LOG.debug_f("Station name: %s", stationName.c_str());

  // Get request parameters
  // Get the date parameter
  std::string date = getQueryParameter("start", "2022-12-25");
  // Get limit parameter with default value
  int limit = std::stoi(getQueryParameter("page", "1"));
  // Get page parameter with default value
  int pagesize = std::stoi(getQueryParameter("pagesize", "12"));


  std::vector<RainfallReading> rainfall_readings =
      s_floodRepository->getStationRainfallReadings(date.c_str(), stationName.c_str(), limit, pagesize);

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
  // More strict pattern: must start and end with alphanumeric, can have hyphens in between

  // m_server.on("^\\/rainfall\\/([a-z0-9-]+)$", HTTP_GET, [this]() { this->riverStation(); });
  //  TODO: There is a better way to do this but time constraints
  m_server.onNotFound(
      [this]()
      {
        String uri = m_server.uri();
        if (uri.startsWith("/rainfall/"))
        {
          auto start = std::chrono::system_clock::now();

          this->riverStation();

          auto end = std::chrono::system_clock::now();
          auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
          LOG.info_f("/river completed in %d milliseconds", duration.count());
        }
        else
        {
          m_server.send(404, "application/json", "{\"error\": \"Route not found\"}");
        }
      });

  // Begin server
  LOG.debug("Starting server in FloodRoutes");
  m_server.begin();
}

void FloodRoutes::loop() { m_server.handleClient(); }
