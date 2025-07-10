//
// Created by Jamie Briggs on 10/07/2025.
//

#include <Arduino.h>
#include <LiquidCrystal.h>

#include "Printer.h"

Printer printer;

void setup() {
    // put your setup code here, to run once:
    Serial.begin(9600);
}

void loop() {
    printer.println("Hello World", FLASH);
    printer.println("Hello World", SCROLL);
    printer.println("Hello", "World", FLASH);
    printer.println("Hello", "World", SCROLL);

}