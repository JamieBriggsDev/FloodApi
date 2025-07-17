//
// Created by Jamie Briggs on 15/07/2025.
//

#include <gmock/gmock.h>
#include <gtest/gtest.h>


#include "display/ILiquidCrystalAdapter.h"
#include "display/LCDDisplay.h"
#include "display/LCDDisplay.cpp"


class MockLiquidCrystal : public ILiquidCrystalAdapter
{
  public:
  MOCK_METHOD(void, clear, (), (override));
  MOCK_METHOD(void, print, (const char*), (override));
  MOCK_METHOD(void, setCursor, (uint8_t, uint8_t), (override));
  MOCK_METHOD(void, begin, (uint8_t, uint8_t), (override));
  MOCK_METHOD(void, scrollDisplayLeft, (), (override));
};


// Test fixture
class LCDDisplayTests : public ::testing::Test
{
  protected:
  testing::NiceMock<MockLiquidCrystal>* mockLcd;
  LCDDisplay* printer;

  void SetUp() override
  {
    mockLcd = new testing::NiceMock<MockLiquidCrystal>();
    printer = new LCDDisplay(*mockLcd);
  }

  void TearDown() override
  {
    delete printer;
    delete mockLcd;
  }
};

// Test clearDisplay
TEST_F(LCDDisplayTests, ClearDisplay)
{
  // Given
  EXPECT_CALL(*mockLcd, clear()).Times(1);

  // When
  printer->clearDisplay();
}

TEST_F(LCDDisplayTests, displayText_OneRow_Sticky)
{
  // Given
  EXPECT_CALL(*mockLcd, setCursor(0, 0)).Times(1);
  EXPECT_CALL(*mockLcd, setCursor(0, 1)).Times(1);
  EXPECT_CALL(*mockLcd, print(testing::StrEq("Row one"))).Times(1);
  EXPECT_CALL(*mockLcd, print(testing::StrEq(""))).Times(1);

  // When
  printer->displayText("Row one", STICKY);
}

TEST_F(LCDDisplayTests, displayText_TwoRows_Sticky)
{
  // Given
  EXPECT_CALL(*mockLcd, setCursor(0, 0)).Times(1);
  EXPECT_CALL(*mockLcd, setCursor(0, 1)).Times(1);
  EXPECT_CALL(*mockLcd, print(testing::StrEq("Row one"))).Times(1);
  EXPECT_CALL(*mockLcd, print(testing::StrEq("Row two"))).Times(1);

  // When
  printer->displayText("Row one", "Row two", STICKY);
}

TEST_F(LCDDisplayTests, displayText_OneRow_Flash)
{
  // Given
  // Both called twice for initial FLASH then STICKY call.
  EXPECT_CALL(*mockLcd, setCursor(0, 0)).Times(2);
  EXPECT_CALL(*mockLcd, setCursor(0, 1)).Times(2);
  EXPECT_CALL(*mockLcd, print(testing::StrEq("Row one"))).Times(1);
  // This is called an additional 2 times for the stick call afterwards
  EXPECT_CALL(*mockLcd, print(testing::StrEq(""))).Times(3);
  // This is called to reset the display for both the flash message, and the original sticky message
  EXPECT_CALL(*mockLcd, clear()).Times(2);

  // When
  printer->displayText("Row one", FLASH);
}


TEST_F(LCDDisplayTests, displayText_OneRow_Scroll)
{
  // Given
  EXPECT_CALL(*mockLcd, setCursor(printer->SCROLL_START_POSITION, 0)).Times(1);
  EXPECT_CALL(*mockLcd, setCursor(printer->SCROLL_START_POSITION, 1)).Times(1);
  EXPECT_CALL(*mockLcd, print(testing::StrEq("Row one"))).Times(1);
  // This is called an additional 2 times for the stick call afterwards
  EXPECT_CALL(*mockLcd, print(testing::StrEq(""))).Times(3);
  EXPECT_CALL(*mockLcd, scrollDisplayLeft()).Times(printer->SCROLL_LENGTH);
  // This is called to reset the display for both the scroll message, and the original sticky message
  EXPECT_CALL(*mockLcd, clear()).Times(2);
  // STICKY is called automatically after scroll, check this also
  EXPECT_CALL(*mockLcd, setCursor(0, 0)).Times(1);
  EXPECT_CALL(*mockLcd, setCursor(0, 1)).Times(1);

  // When
  printer->displayText("Row one", SCROLL);
}

TEST_F(LCDDisplayTests, displayText_TwoRows_Scroll)
{
  // Given
  EXPECT_CALL(*mockLcd, setCursor(printer->SCROLL_START_POSITION, 0)).Times(1);
  EXPECT_CALL(*mockLcd, setCursor(printer->SCROLL_START_POSITION, 1)).Times(1);
  EXPECT_CALL(*mockLcd, print(testing::StrEq("Row one"))).Times(1);
  EXPECT_CALL(*mockLcd, print(testing::StrEq("Row two"))).Times(1);
  // This is called an additional 2 times for the stick call afterwards
  EXPECT_CALL(*mockLcd, scrollDisplayLeft()).Times(printer->SCROLL_LENGTH);
  // This is called to reset the display for both the scroll message, and the original sticky message
  EXPECT_CALL(*mockLcd, clear()).Times(2);
  // STICKY is called automatically after scroll, check this also
  EXPECT_CALL(*mockLcd, print(testing::StrEq(""))).Times(2);
  EXPECT_CALL(*mockLcd, setCursor(0, 0)).Times(1);
  EXPECT_CALL(*mockLcd, setCursor(0, 1)).Times(1);

  // When
  printer->displayText("Row one", "Row two", SCROLL);
}


