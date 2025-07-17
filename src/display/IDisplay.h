//
// Created by Jamie Briggs on 16/07/2025.
//

#ifndef IDISPLAY_H
#define IDISPLAY_H

enum PrintType
{
  SCROLL,
  FLASH,
  STICKY
};

class IDisplay
{
public:
  virtual ~IDisplay() = default;
  /**
   * Prints two rows of text onto an LCD screen. If print type is not STICKY, after displaying the message, it will
   * display the previous sticky message.
   *
   * @param rowOne First row of text to display
   * @param rowTwo Second row of text to display (Default is STICKY)
   * @param printType How the text should be displayed
   */
  virtual void displayText(const char* rowOne, const char* rowTwo, PrintType printType = STICKY) = 0;
  /**
     * Prints a single row of text onto an LCD screen. If print type is not STICKY, after displaying the message,
     * it will display the previous sticky message.
     *
     * @param rowOne Row of text to display
     * @param printType How the text should be displayed (Default is FLASH)
     */
  virtual void displayText(const char* rowOne, PrintType printType = STICKY) = 0;
  /**
     * Clears the current content displayed on the LCD screen.
     * This resets the display to an empty state.
     */
  virtual void clearDisplay() const = 0;
};
#endif //IDISPLAY_H
