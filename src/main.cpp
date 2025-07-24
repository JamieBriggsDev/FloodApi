//
// Created by Jamie Briggs on 10/07/2025.
//

#include <Arduino.h>
#include <sstream>

#include "../src/display/LCDDisplay.h"
#include "../src/display/LiquidCrystalAdapter.h"
#include "FloodMapper.h"
#include "FloodRoutes.h"
#include "db/FloodRepository.h"
#include "def_pin_outs.h"
#include "def_wifi_settings.h"
#include "logger/def_logger_factory.h"




FloodRoutes* flood_routes;
LiquidCrystalAdapter* lcd;
LCDDisplay* display;
IFloodRepository* flood_repository;
IFloodMapper* flood_mapper;

#define REPO_ENABLED 1
#define ROUTES_ENABLED 1

void setup()
{
  Serial.begin(115200);
  LOG.info("Setting up Flood API...");

  LOG.info("Checking defragmentation...");
  // In your setup(), before initializing your components
  if (heap_caps_check_integrity_all(true))
  {
    LOG.debug("Heap integrity check passed");
  }


  LOG.debug("Initializing LCD...");
  lcd = new LiquidCrystalAdapter(ESP_RS_PIN, ESP_ENABLE_PIN, ESP_D0_PIN, ESP_D1_PIN, ESP_D2_PIN, ESP_D3_PIN);
  LOG.debug("Initializing Printer Service...");
  display = new LCDDisplay(*lcd);
  display->displayText("Starting", "Flood App!", STICKY);
  LOG.debug_f("Initial Free Heap: %d bytes", ESP.getFreeHeap());

#if REPO_ENABLED

  LOG.debug("Creating Flood repository...");
  flood_repository = new FloodRepository("/flood_downgraded.db");

#endif

  LOG.debug("Initializing Flood mapper...");
  flood_mapper = new FloodMapper();

#if ROUTES_ENABLED
  // Why does example setup wifi like this?
  /**
  pinMode(led, OUTPUT);
  digitalWrite(led, 0);
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Serial.println("");
   */
  LOG.debug_f("Connecting to WiFi: %s", WIFI_SSID);
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFiClass::status() != WL_CONNECTED)
  {
    delay(500);
    display->displayText("Connecting..", FLASH);
  }

  // Display IP and PORT number
  std::ostringstream portMessage;
  portMessage << "Port: " << std::to_string(PORT);
  display->displayText(WiFi.localIP().toString().c_str(), portMessage.str().c_str(), STICKY);

  LOG.debug("Initializing Flood routes...");
  flood_routes = new FloodRoutes(display, flood_repository, flood_mapper);
#endif


#if REPO_ENABLED && true

  LOG.debug("Initializing Flood repository...");
  // This is initialized after FloodRoutes has initialized
  flood_repository->init();

#endif

  LOG.info("Completed setup!");
}

void loop()
{
  // Monitor heap memory usage
  LOG.debug_f("Free Heap: %d bytes, Largest Free Block: %d bytes", ESP.getFreeHeap(), ESP.getMaxAllocHeap());

#if ROUTES_ENABLED
  flood_routes->loop();
#endif


#if REPO_ENABLED && false
  const auto riverReadings = flood_repository->getRiverReadings("2022-12-12", 1, 10);
  // This will log the JSON for me
  flood_mapper->getFloodData(riverReadings);
#endif

  delay(5000); // Wait 5 seconds
}

void cleanup()
{
  delete lcd;
  delete display;
  delete flood_repository;
  delete flood_mapper;
  delete flood_routes;
}
