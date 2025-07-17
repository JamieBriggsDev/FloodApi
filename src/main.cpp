//
// Created by Jamie Briggs on 10/07/2025.
//

#include <Arduino.h>

#include "../include/display/LCDDisplay.h"
#include "../include/display/LiquidCrystalAdapter.h"
#include "FloodRoutes.h"
#include "def_pin_outs.h"


FloodRoutes* flood_routes;
LiquidCrystalAdapter* lcd;
LCDDisplay* printer;

void setup()
{
  Serial.begin(115200);
  lcd = new LiquidCrystalAdapter(ESP_RS_PIN, ESP_ENABLE_PIN, ESP_D0_PIN, ESP_D1_PIN, ESP_D2_PIN, ESP_D3_PIN);
  printer = new LCDDisplay(*lcd);
  printer->displayText("Starting", "Flood App!", STICKY);
  flood_routes = new FloodRoutes(printer);
}

void loop() { flood_routes->loop(); }
