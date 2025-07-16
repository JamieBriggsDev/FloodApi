//
// Created by Jamie Briggs on 10/07/2025.
//

#ifndef PRINTER_H
#define PRINTER_H

#include "ILiquidCrystalAdapter.h"

#include <string>

#include "IDisplay.h"


/**
 * Handles operations for printing text on an LCD screen.
 * Manages various display modes such as flashing or scrolling and supports
 * both single-line and two-line text printing.
 */
class LCDDisplay : public IDisplay
{
  std::string m_stickyTextRowOne;
  std::string m_stickyTextRowTwo;

  // Reference to interface
  ILiquidCrystalAdapter& m_lcd;

  void displayStickyMessage() const;
  void displayFlashMessage(const char* rowOne, const char* rowTwo) const;
  void displayScrollMessage(const char* rowOne, const char* rowTwo) const;

public:
  static constexpr uint8_t LCD_COLUMNS = 16;
  static constexpr uint8_t LCD_ROWS = 2;
  static constexpr uint8_t SCROLL_START_POSITION = LCD_COLUMNS + 1;
  static constexpr uint8_t SCROLL_LENGTH = 32;
  static constexpr int FLASH_DISPLAY_TIME_MS = 3000;
  static constexpr int SCROLL_DELAY_MS = 250;
  static constexpr int SCROLL_PAUSE_DELAY_MS = 1000;

  explicit LCDDisplay(ILiquidCrystalAdapter& lcd) : m_lcd(lcd)
  {
    m_lcd.begin(LCD_COLUMNS, LCD_ROWS);
    LCDDisplay::displayText("", "", STICKY);
  }

  /**
   * Prints two rows of text onto an LCD screen. If print type is not STICKY, after displaying the message, it will
   * display the previous sticky message.
   *
   * @param rowOne First row of text to display
   * @param rowTwo Second row of text to display (Default is STICKY)
   * @param printType How the text should be displayed
   */
  void displayText(const char* rowOne, const char* rowTwo, PrintType printType = STICKY) override;

  /**
   * Prints a single row of text onto an LCD screen. If print type is not STICKY, after displaying the message,
   * it will display the previous sticky message.
   *
   * @param rowOne Row of text to display
   * @param printType How the text should be displayed (Default is FLASH)
   */
  void displayText(const char* rowOne, PrintType printType = STICKY) override;

  /**
   * Clears the current content displayed on the LCD screen.
   * This resets the display to an empty state.
   */
  void clearDisplay() const override;
};


#endif // PRINTER_H
