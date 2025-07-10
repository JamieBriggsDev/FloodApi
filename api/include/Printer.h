//
// Created by Jamie Briggs on 10/07/2025.
//

#ifndef PRINTER_H
#define PRINTER_H
#include <LiquidCrystal.h>

enum PrintType
{
    SCROLL,
    FLASH
};

/**
 * Handles operations for printing text on an LCD screen.
 * Manages various display modes such as flashing or scrolling and supports
 * both single-line and two-line text printing.
 */
class Printer
{
private:
    static constexpr uint8_t LCD_COLUMNS = 16;
    static constexpr uint8_t LCD_ROWS = 2;
    static constexpr uint8_t SCROLL_START_POSITION = LCD_COLUMNS + 1;
    static constexpr uint8_t SCROLL_LENGTH = 32;
    static constexpr int FLASH_DISPLAY_TIME_MS = 3000;
    static constexpr int SCROLL_DELAY_MS = 200;
    static constexpr int SCROLL_PAUSE_DELAY_MS = 1000;

    static constexpr int ESP_RS_PIN = 4;
    static constexpr int ESP_ENABLE_PIN = 12;
    static constexpr int ESP_D0_PIN = 3;
    static constexpr int ESP_D1_PIN = 1;
    static constexpr int ESP_D2_PIN = 25;
    static constexpr int ESP_D3_PIN = 26;

    LiquidCrystal m_lcd;

    void displayFlashMessage(const char* rowOne, const char* rowTwo);
    void displayScrollMessage(const char* rowOne, const char* rowTwo);

public:
    Printer() : m_lcd(ESP_RS_PIN, ESP_ENABLE_PIN, ESP_D0_PIN, ESP_D1_PIN, ESP_D2_PIN, ESP_D3_PIN)
    {
        m_lcd.begin(LCD_COLUMNS, LCD_ROWS);
    }

    /**
     * Prints two rows of text onto an LCD screen.
     *
     * @param rowOne First row of text to display
     * @param rowTwo Second row of text to display (Default is FLASH)
     * @param printType How the text should be displayed
     */
    void println(const char* rowOne, const char* rowTwo, PrintType printType = FLASH);

    /**
     * Prints a single row of text onto an LCD screen.
     *
     * @param rowOne Row of text to display
     * @param printType How the text should be displayed (Default is FLASH)
     */
    void println(const char* rowOne, PrintType printType = FLASH);
};


#endif //PRINTER_H
