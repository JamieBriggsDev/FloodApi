//
// Created by Jamie Briggs on 10/07/2025.
//

#include <Arduino.h>

#include "../src/display/LCDDisplay.h"
#include "../src/display/LiquidCrystalAdapter.h"
#include "FloodRoutes.h"
#include "db/FloodRepository.h"
#include "def_pin_outs.h"


FloodRoutes* flood_routes;
LiquidCrystalAdapter* lcd;
LCDDisplay* printer;
IFloodRepository* flood_repository;

void setup()
{
  Serial.begin(115200);
  lcd = new LiquidCrystalAdapter(ESP_RS_PIN, ESP_ENABLE_PIN, ESP_D0_PIN, ESP_D1_PIN, ESP_D2_PIN, ESP_D3_PIN);
  printer = new LCDDisplay(*lcd);
  printer->displayText("Starting", "Flood App!", STICKY);

  flood_repository = new FloodRepository();

  flood_routes = new FloodRoutes(printer, flood_repository);
}

void loop() { flood_routes->loop(); }
