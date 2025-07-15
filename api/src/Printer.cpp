//
// Created by Jamie Briggs on 10/07/2025.
//

#include "../include/Printer.h"

#if defined(ARDUINO)
#include <Arduino.h>
#endif

#include "LiquidCrystalAdapter.h"

void Printer::displayStickyMessage()
{
#if defined(ARDUINO)
    Serial.println(m_stickyTextRowOne.c_str());
    Serial.println(m_stickyTextRowTwo.c_str());
#endif

    m_lcd.setCursor(0, 0);
    m_lcd.print(m_stickyTextRowOne.c_str());
    m_lcd.setCursor(0, 1);
    m_lcd.print(m_stickyTextRowTwo.c_str());
}

void Printer::displayFlashMessage(const char* rowOne, const char* rowTwo)
{
#if defined(ARDUINO)
    Serial.println(rowOne);
    Serial.println(rowTwo);
#endif

    m_lcd.setCursor(0, 0);
    m_lcd.print(rowOne);
    m_lcd.setCursor(0, 1);
    m_lcd.print(rowTwo);
#if defined(ARDUINO)
    delay(FLASH_DISPLAY_TIME_MS);
#endif
    m_lcd.clear();
}

void Printer::displayScrollMessage(const char* rowOne, const char* rowTwo)
{
#if defined(ARDUINO)
    Serial.println(rowOne);
    Serial.println(rowTwo);
#endif
    m_lcd.setCursor(SCROLL_START_POSITION, 0);
    m_lcd.print(rowOne);
    m_lcd.setCursor(SCROLL_START_POSITION, 1);
    m_lcd.print(rowTwo);
    // Scroll for twice the length of display
    for (int pos = 0; pos < SCROLL_LENGTH; pos++)
    {
        m_lcd.scrollDisplayLeft();
#if defined(ARDUINO)
        delay(SCROLL_DELAY_MS);
#endif
    }
#if defined(ARDUINO)
    delay(SCROLL_PAUSE_DELAY_MS);
#endif
    m_lcd.clear();
}

void Printer::clearDisplay()
{
    m_lcd.clear();
}

void Printer::println(const char* rowOne, const char* rowTwo, PrintType printType)
{
    m_lcd.clear();
    if (printType == FLASH)
    {
        this->displayFlashMessage(rowOne, rowTwo);
#if defined(ARDUINO)
        // Helps to separate messages in monitor output
        Serial.println("\n~~~~~~~~~~~~~~~\n");
#endif
    }
    else if (printType == STICKY)
    {
        this->m_stickyTextRowOne = rowOne;
        this->m_stickyTextRowTwo = rowTwo;
    }
    else
    {
        this->displayScrollMessage(rowOne, rowTwo);
#if defined(ARDUINO)
        // Helps to separate messages in monitor output
        Serial.println("\n~~~~~~~~~~~~~~~\n");
#endif
    }
    this->displayStickyMessage();
#if defined(ARDUINO)
    // Helps to separate messages in monitor output
    Serial.println("\n~~~~~~~~~~~~~~~\n");
#endif
}


void Printer::println(const char* rowOne, PrintType printType)
{
    m_lcd.clear();
    this->println(rowOne, "", printType);
}
