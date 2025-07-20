//
// Created by Jamie Briggs on 10/07/2025.
//

#include "FloodRoutes.h"

#include <Update.h>
#include <aWOT.h>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>

#include "def_wifi_settings.h"
#include "display/IDisplay.h"
#include "logger/def_logger_factory.h"


// Add static member definition
IDisplay* FloodRoutes::s_display = nullptr;
IFloodRepository* FloodRoutes::s_floodRepository = nullptr;

static std::string getMethodName(Request::MethodType method)
{
  switch (method)
  {
  case Request::GET:
    return "GET";
  case Request::POST:
    return "PST";
  case Request::PUT:
    return "PUT";
  case Request::PATCH:
    return "PTC";
  case Request::DELETE:
    return "DEL";
  default:
    return "UNK";
  }
}

void FloodRoutes::river(Request& request, Response& response)
{
  LOG.info("/river requested");
  auto start = std::chrono::system_clock::now();
  response.set("Content-Type", "application/json");

  std::vector<RiverReading> river_readings = s_floodRepository->getRiverReadings("2025-12-25");



  auto end = std::chrono::system_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
  response.set("Request-Time", std::to_string(duration.count()).c_str());
}

void FloodRoutes::riverStation(Request& request, Response& response)
{
  response.set("Content-Type", "application/json");
}

void FloodRoutes::logRequest(Request& req, Response& res)
{
  std::ostringstream messageRowOne;
  messageRowOne << "Request: ";
  std::ostringstream messageRowTwo;
  messageRowTwo << getMethodName(req.method()) << ": " << req.path();
  s_display->displayText(messageRowOne.str().c_str(), messageRowTwo.str().c_str(), FLASH);
}

FloodRoutes::FloodRoutes(IDisplay* display, IFloodRepository* flood_repository) : m_server(PORT)
{
  s_display = display;
  s_floodRepository = flood_repository;

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    s_display->displayText("Connecting..", FLASH);
  }
  // Display IP and PORT number
  std::ostringstream portMessage;
  portMessage << "Port: " << std::to_string(PORT);
  s_display->displayText(WiFi.localIP().toString().c_str(), portMessage.str().c_str(), STICKY);

  // m_app.header("Expect", m_expectHeader, 20);
  //  Loging request middleware
  m_app.use(&logRequest);

  // River API endpoints
  m_app.get("/river", &river);
  m_app.get("/river/{station}", &riverStation);

  // Begin server
  LOG.info("Starting server");
  m_server.begin();
}

void FloodRoutes::loop()
{
  WiFiClient client = m_server.available();

  if (client.connected())
  {
    m_app.process(&client);
    client.stop();
  }
}
