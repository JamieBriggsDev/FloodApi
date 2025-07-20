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
#include "logger/def_logger_factory.h"


FloodRoutes* flood_routes;
LiquidCrystalAdapter* lcd;
LCDDisplay* printer;
IFloodRepository* flood_repository;
IFloodMapper* flood_mapper;

void setup()
{
  Serial.begin(115200);
  LOG.debug("Starting setup...");

  LOG.debug("Initializing LCD...");
  lcd = new LiquidCrystalAdapter(ESP_RS_PIN, ESP_ENABLE_PIN, ESP_D0_PIN, ESP_D1_PIN, ESP_D2_PIN, ESP_D3_PIN);
  LOG.debug("Initializing Printer Service...");
  printer = new LCDDisplay(*lcd);
  printer->displayText("Starting", "Flood App!", STICKY);

  LOG.debug("Initializing Flood repository...");
  flood_repository = new FloodRepository();
  LOG.debug("Initializing Flood mapper...");
  flood_mapper = new FloodMapper();

  LOG.debug("Initializing Flood routes...");
  flood_routes = new FloodRoutes(printer, flood_repository, flood_mapper);
}

void loop() { flood_routes->loop(); }
