//
// Created by Jamie Briggs on 10/07/2025.
//

#include "display/LCDDisplay.h"

#if defined(ARDUINO)
#include <Arduino.h>
#endif

#include "logger/def_logger_factory.h"

void LCDDisplay::displayStickyMessage() const
{

  m_lcd.setCursor(0, 0);
  m_lcd.print(m_stickyTextRowOne.c_str());
  m_lcd.setCursor(0, 1);
  m_lcd.print(m_stickyTextRowTwo.c_str());
}

void LCDDisplay::displayFlashMessage(const char* rowOne, const char* rowTwo) const
{
  LOG.info(rowOne);
  LOG.info(rowTwo);

  m_lcd.setCursor(0, 0);
  m_lcd.print(rowOne);
  m_lcd.setCursor(0, 1);
  m_lcd.print(rowTwo);
#if defined(ARDUINO)
  delay(FLASH_DISPLAY_TIME_MS);
#endif
  m_lcd.clear();
}

void LCDDisplay::displayScrollMessage(const char* rowOne, const char* rowTwo) const
{
  LOG.info(rowOne);
  LOG.info(rowTwo);

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

void LCDDisplay::clearDisplay() const { m_lcd.clear(); }

void LCDDisplay::displayText(const char* rowOne, const char* rowTwo, PrintType printType)
{
  m_lcd.clear();
  if (printType == FLASH)
  {
    this->displayFlashMessage(rowOne, rowTwo);
    // Helps to separate messages in monitor output
    LOG.info("\n~~~~~~~~~~~~~~~\n");
  }
  else if (printType == STICKY)
  {
    // Helps to separate messages in monitor output
    LOG.info("\n~~~~~~~~~~~~~~~\n");
    LOG.info(m_stickyTextRowOne.c_str());
    LOG.info(m_stickyTextRowTwo.c_str());
    this->m_stickyTextRowOne = rowOne;
    this->m_stickyTextRowTwo = rowTwo;
    // Helps to separate messages in monitor output
    LOG.info("\n~~~~~~~~~~~~~~~\n");
  }
  else
  {
    // Helps to separate messages in monitor output
    LOG.info("\n~~~~~~~~~~~~~~~\n");
    this->displayScrollMessage(rowOne, rowTwo);
    // Helps to separate messages in monitor output
    LOG.info("\n~~~~~~~~~~~~~~~\n");
  }
  this->displayStickyMessage();

}


void LCDDisplay::displayText(const char* rowOne, PrintType printType)
{
  LOG.debug("Clearing liquid display");
  this->displayText(rowOne, "", printType);
}
