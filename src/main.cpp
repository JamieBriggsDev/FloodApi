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

void setup()
{
  Serial.begin(115200);
  LOG.info("Setting up Flood API...");

  LOG.debug("Initializing LCD...");
  lcd = new LiquidCrystalAdapter(ESP_RS_PIN, ESP_ENABLE_PIN, ESP_D0_PIN, ESP_D1_PIN, ESP_D2_PIN, ESP_D3_PIN);
  LOG.debug("Initializing Printer Service...");
  display = new LCDDisplay(*lcd);
  display->displayText("Starting", "Flood App!", STICKY);

  LOG.debug_f("Connecting to WiFi: %s", WIFI_SSID);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFiClass::status() != WL_CONNECTED)
  {
    delay(500);
    display->displayText("Connecting..", FLASH);
  }

  LOG.debug("Initializing Flood repository...");
  flood_repository = new FloodRepository();
  LOG.debug("Initializing Flood mapper...");
  flood_mapper = new FloodMapper();

  LOG.debug("Initializing Flood routes...");
  flood_routes = new FloodRoutes(display, flood_repository, flood_mapper);
  LOG.info("Completed setup!");
}

void loop() { flood_routes->loop(); }
