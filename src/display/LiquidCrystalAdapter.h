//
// Created by Jamie Briggs on 15/07/2025.
//

#ifndef LIQUIDCRYSTAL_ADAPTER_H
#define LIQUIDCRYSTAL_ADAPTER_H

#include <LiquidCrystal.h>

#include "ILiquidCrystalAdapter.h"


class LiquidCrystalAdapter final : public ILiquidCrystalAdapter
{
  LiquidCrystal m_lcd;

  public:
  LiquidCrystalAdapter(const uint8_t rs, const uint8_t enable, const uint8_t d0, const uint8_t d1, const uint8_t d2,
                           const uint8_t d3) :
      m_lcd(rs, enable, d0, d1, d2, d3)
  {
  }

  void clear() override { m_lcd.clear(); }
  void print(const std::string text) override { m_lcd.print(text.c_str()); }
  void setCursor(const uint8_t col, const uint8_t row) override { m_lcd.setCursor(col, row); }
  void begin(const uint8_t cols, const uint8_t rows) override { m_lcd.begin(cols, rows); }
  void scrollDisplayLeft() override { m_lcd.scrollDisplayLeft(); }
};

#endif // LIQUIDCRYSTAL_ADAPTER_H
