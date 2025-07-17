//
// Created by Jamie Briggs on 16/07/2025.
//

#ifndef ILIQUIDCRYSTAL_H
#define ILIQUIDCRYSTAL_H

#include <cstdint>

class ILiquidCrystalAdapter {
public:
    virtual ~ILiquidCrystalAdapter() = default;
    virtual void clear() = 0;
    virtual void print(const char*) = 0;
    virtual void setCursor(uint8_t, uint8_t) = 0;
    virtual void begin(uint8_t, uint8_t) = 0;
    virtual void scrollDisplayLeft() = 0;
};


#endif //ILIQUIDCRYSTAL_H
