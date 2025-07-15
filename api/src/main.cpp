//
// Created by Jamie Briggs on 10/07/2025.
//

#include <Arduino.h>

#include "FloodRoutes.h"
#include "PinOuts.h"
#include "Printer.h"


FloodRoutes* flood_routes;
Printer* printer;

void setup()
{
    Serial.begin(115200);
    LiquidCrystalAdapter lcd(ESP_RS_PIN, ESP_ENABLE_PIN, ESP_D0_PIN, ESP_D1_PIN, ESP_D2_PIN, ESP_D3_PIN);
    printer = new Printer(lcd);
    printer->println("Starting", "Flood App!", STICKY);
    flood_routes = new FloodRoutes(printer);
}

void loop() { flood_routes->loop(); }
