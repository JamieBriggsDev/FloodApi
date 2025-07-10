//
// Created by Jamie Briggs on 10/07/2025.
//

#include <Arduino.h>

#include "FloodRoutes.h"
#include "Printer.h"

FloodRoutes* flood_routes;
Printer* printer;

void setup()
{
    Serial.begin(115200);
    printer = new Printer();
    printer->println("Starting",
                                   "Flood App!", FLASH);
    flood_routes = new FloodRoutes(printer);
}

void loop()
{
    flood_routes->loop();
}
