//
// Created by Jamie Briggs on 15/07/2025.
//

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "../include/Printer.h"
#include "../src/Printer.cpp"


#if defined(ARDUINO)
#include <Arduino.h>

// Mock for LiquidCrystal
class MockLCD : public LiquidCrystal{
public:
    // Add constructor that matches LiquidCrystal's constructor
    MockLCD() : LiquidCrystal(0, 1, 2, 3, 4, 5) {  // Dummy pin values
    }

    MOCK_METHOD(void, setCursor, (int, int));
    MOCK_METHOD(void, print, (const char*));
    MOCK_METHOD(void, clear, ());
    MOCK_METHOD(void, scrollDisplayLeft, ());
};


// Test fixture
class PrinterTest : public ::testing::Test {
protected:
    MockLCD mockLcd;
    Printer* printer;

    void SetUp() override {
        printer = new Printer(static_cast<LiquidCrystal>(mockLcd));
        // Initialize any needed test data
    }

    void TearDown() override {
        delete printer;
    }
};

// Test displayStickyMessage
TEST_F(PrinterTest, DisplayStickyMessage) {
    // Arrange
    const char* row1 = "Hello";
    const char* row2 = "World";

    // Expect
    EXPECT_CALL(mockLcd, setCursor(0, 0)).Times(1);
    EXPECT_CALL(mockLcd, setCursor(0, 1)).Times(1);
    EXPECT_CALL(mockLcd, print(::testing::StrEq(row1))).Times(1);
    EXPECT_CALL(mockLcd, print(::testing::StrEq(row2))).Times(1);

    // Act
    printer->println(row1, row2, STICKY);
}

// Test displayFlashMessage
TEST_F(PrinterTest, DisplayFlashMessage) {
    // Arrange
    const char* row1 = "Flash";
    const char* row2 = "Test";

    // Expect
    EXPECT_CALL(mockLcd, clear()).Times(2);  // Once at start, once after delay
    EXPECT_CALL(mockLcd, setCursor(0, 0)).Times(1);
    EXPECT_CALL(mockLcd, setCursor(0, 1)).Times(1);
    EXPECT_CALL(mockLcd, print(::testing::StrEq(row1))).Times(1);
    EXPECT_CALL(mockLcd, print(::testing::StrEq(row2))).Times(1);

    // Act
    printer->println(row1, row2, FLASH);
}

// Test displayScrollMessage
TEST_F(PrinterTest, DisplayScrollMessage) {
    // Arrange
    const char* row1 = "Scroll";
    const char* row2 = "Message";

    // Expect
    EXPECT_CALL(mockLcd, clear()).Times(2);  // Once at start, once after scrolling
    EXPECT_CALL(mockLcd, setCursor(Printer::SCROLL_START_POSITION, 0)).Times(1);
    EXPECT_CALL(mockLcd, setCursor(Printer::SCROLL_START_POSITION, 1)).Times(1);
    EXPECT_CALL(mockLcd, print(::testing::StrEq(row1))).Times(1);
    EXPECT_CALL(mockLcd, print(::testing::StrEq(row2))).Times(1);
    EXPECT_CALL(mockLcd, scrollDisplayLeft()).Times(Printer::SCROLL_LENGTH);

    // Act
    printer->println(row1, row2, SCROLL);
}

// Test clearDisplay
TEST_F(PrinterTest, ClearDisplay) {
    // Expect
    EXPECT_CALL(mockLcd, clear()).Times(1);

    // Act
    printer->clearDisplay();
}

// Test single-line print
TEST_F(PrinterTest, SingleLinePrint) {
    // Arrange
    const char* row1 = "Single Line";

    // Expect
    EXPECT_CALL(mockLcd, clear()).Times(1);
    EXPECT_CALL(mockLcd, print(::testing::StrEq(row1))).Times(1);
    EXPECT_CALL(mockLcd, print(::testing::StrEq(""))).Times(1);

    // Act
    printer->println(row1, FLASH);
}

#endif