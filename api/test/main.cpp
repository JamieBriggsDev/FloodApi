//
// Created by Jamie Briggs on 15/07/2025.
//

#include <gtest/gtest.h>
// uncomment line below if you plan to use GMock
// #include <gmock/gmock.h>

// TEST(...)
// TEST_F(...)

#if _WIN32
#include <stdio.h>  // for fdopen
#include <windows.h> // for _MAX_PATH (on Windows)
#endif


#if defined(ARDUINO)
#include <Arduino.h>

#include "gmock/gmock.h"

void setup()
{
    // should be the same value as for the `test_speed` option in "platformio.ini"
    // default value is test_speed=115200
    Serial.begin(115200);

    //::testing::InitGoogleTest();
    // if you plan to use GMock, replace the line above with
    //::testing::InitGoogleMock();

    Serial.println("Starting tests...");
    ::testing::InitGoogleMock();
    Serial.println("Google Mock initialized");

}

bool testsComplete = false;

void loop()
{

    if (!testsComplete)
    {
        RUN_ALL_TESTS();
        testsComplete = true;
        Serial.println("Tests completed");
    }
    delay(1000);

}

#else
int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    // if you plan to use GMock, replace the line above with
    // ::testing::InitGoogleMock(&argc, argv);

    if (RUN_ALL_TESTS())
        ;

    // Always return zero-code and allow PlatformIO to parse results
    return 0;
}
#endif
