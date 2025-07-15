//
// Created by Jamie Briggs on 15/07/2025.
//

#include <gmock/gmock.h>
#include <gtest/gtest.h>


#include "../include/LiquidCrystalAdapter.h"
#include "../include/Printer.h"
#include "../src/Printer.cpp"

class MockLiquidCrystal : public LiquidCrystalAdapter {
public:
    MOCK_METHOD(void, clear, (), (override));
    MOCK_METHOD(void, print, (const char*), (override));
    MOCK_METHOD(void, setCursor, (uint8_t, uint8_t), (override));
    MOCK_METHOD(void, begin, (uint8_t, uint8_t), (override));
    MOCK_METHOD(void, scrollDisplayLeft, (), (override));
};


// Test fixture
class PrinterTest : public ::testing::Test
{
protected:
    MockLiquidCrystal* mockLcd;
    Printer* printer;

    void SetUp() override
    {
        mockLcd = new MockLiquidCrystal();
        printer = new Printer(*mockLcd);
    }

    void TearDown() override
    {
        delete printer;
        delete mockLcd;
    }
};

// Test clearDisplay
TEST_F(PrinterTest, ClearDisplay)
{
    // Given
    printer->clearDisplay();

    // When
    EXPECT_CALL(*mockLcd, clear()).Times(1);
}

