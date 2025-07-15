//
// Created by Jamie Briggs on 15/07/2025.
//

// test/common_tests.cpp
#include <gtest/gtest.h>


// Tests that can run both natively and on ESP32
TEST(test_common, BasicTest) {
    EXPECT_EQ(2 + 2, 4);
}