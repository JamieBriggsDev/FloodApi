//
// Created by Jamie Briggs on 15/07/2025.
//

#ifndef LIQUIDCRYSTAL_ADAPTER_H
#define LIQUIDCRYSTAL_ADAPTER_H

#include <LiquidCrystal.h>

#include "ILiquidCrystalAdapter.h"


class LiquidCrystalAdapter : public ILiquidCrystalAdapter
{
  LiquidCrystal m_lcd;

  public:
  LiquidCrystalAdapter(uint8_t rs, uint8_t enable, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3) :
      m_lcd(rs, enable, d0, d1, d2, d3)
  {
  }

  void clear() override { m_lcd.clear(); }
  void print(const char* text) override { m_lcd.print(text); }
  void setCursor(uint8_t col, uint8_t row) override { m_lcd.setCursor(col, row); }
  void begin(uint8_t cols, uint8_t rows) override { m_lcd.begin(cols, rows); }
  void scrollDisplayLeft() override { m_lcd.scrollDisplayLeft(); }
};

#endif // LIQUIDCRYSTAL_ADAPTER_H
