//
// Created by Jamie Briggs on 15/07/2025.
//

// test/hardware_tests.cpp
#include <gtest/gtest.h>

#if defined(ARDUINO)
#include <Arduino.h>

// Tests that require Arduino hardware
TEST(test_hardware, PinTest) {
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, HIGH);
    EXPECT_EQ(digitalRead(LED_BUILTIN), HIGH);
    Serial.println("Pin test passed");
}
#endif