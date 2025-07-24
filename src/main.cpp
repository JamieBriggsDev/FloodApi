//
// Created by Jamie Briggs on 10/07/2025.
//

#include <Arduino.h>

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

#define REPO_ENABLED 0
#define ROUTES_ENABLED 1

void setup()
{
  Serial.begin(115200);
  LOG.info("Setting up Flood API...");

  LOG.debug("Initializing LCD...");
  lcd = new LiquidCrystalAdapter(ESP_RS_PIN, ESP_ENABLE_PIN, ESP_D0_PIN, ESP_D1_PIN, ESP_D2_PIN, ESP_D3_PIN);
  LOG.debug("Initializing Printer Service...");
  display = new LCDDisplay(*lcd);
  display->displayText("Starting", "Flood App!", STICKY);
  LOG.debug_f("Initial Free Heap: %d bytes", ESP.getFreeHeap());

#if REPO_ENABLED

  LOG.debug("Initializing Flood repository...");
  flood_repository = new FloodRepository("/small_flood_downgraded.db");
  flood_repository->init();

#endif

  LOG.debug("Initializing Flood mapper...");
  flood_mapper = new FloodMapper();

#if ROUTES_ENABLED
  LOG.debug_f("Connecting to WiFi: %s", WIFI_SSID);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFiClass::status() != WL_CONNECTED)
  {
    delay(500);
    display->displayText("Connecting..", FLASH);
  }

  LOG.debug("Initializing Flood routes...");
  flood_routes = new FloodRoutes(display, flood_repository, flood_mapper);
#endif

  LOG.info("Completed setup!");
}

void loop()
{
  // Monitor heap memory usage
  LOG.debug_f("Free Heap: %d bytes", ESP.getFreeHeap());
  LOG.debug_f("Free Memory:", String(ESP.getFreeHeap()).c_str());

#if ROUTES_ENABLED
  flood_routes->loop();
#endif


#if REPO_ENABLED
  const auto riverReadings = flood_repository->getRiverReadings("2022-12-12", 1, 10);
  // This will log the JSON for me
  flood_mapper->getFloodData(riverReadings);
#endif

  delay(5000); // Wait 5 seconds
}

void cleanup() {
  delete lcd;
  delete display;
  delete flood_repository;
  delete flood_mapper;
  delete flood_routes;
}

