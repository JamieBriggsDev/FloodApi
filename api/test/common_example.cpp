//
// Created by Jamie Briggs on 15/07/2025.
//

// test/common_tests.cpp
#include <gtest/gtest.h>

#if _WIN32
#include <stdio.h> // for fdopen
#include <windows.h> // for _MAX_PATH (on Windows)
#endif

// Tests that can run both natively and on ESP32
TEST(test_common, BasicTest) { EXPECT_EQ(2 + 2, 4); }
