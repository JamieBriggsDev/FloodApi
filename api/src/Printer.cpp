//
// Created by Jamie Briggs on 10/07/2025.
//

#include "../include/Printer.h"

#include "LiquidCrystal.h"

void Printer::displayStickyMessage(const char* rowOne, const char* rowTwo)
{
    m_lcd.setCursor(0, 0);
    m_lcd.print(rowOne);
    m_lcd.setCursor(0, 1);
    m_lcd.print(rowTwo);
}

void Printer::displayFlashMessage(const char* rowOne, const char* rowTwo)
{
    this->displayStickyMessage(rowOne, rowTwo);
    delay(FLASH_DISPLAY_TIME_MS);
    m_lcd.clear();
}

void Printer::displayScrollMessage(const char* rowOne, const char* rowTwo)
{
    m_lcd.setCursor(SCROLL_START_POSITION, 0);
    m_lcd.print(rowOne);
    m_lcd.setCursor(SCROLL_START_POSITION, 1);
    m_lcd.print(rowTwo);
    // Scroll for twice the length of display
    for (int pos = 0; pos < SCROLL_LENGTH; pos++)
    {
        m_lcd.scrollDisplayLeft();
        delay(SCROLL_DELAY_MS);
    }
    delay(SCROLL_PAUSE_DELAY_MS);
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
    }
    else if (printType == STICKY)
    {
        this->displayStickyMessage(rowOne, rowTwo);
    }
    else
    {
        this->displayScrollMessage(rowOne, rowTwo);
    }
}


void Printer::println(const char* rowOne, PrintType printType)
{
    m_lcd.clear();
    this->println(rowOne, "", printType);
}
