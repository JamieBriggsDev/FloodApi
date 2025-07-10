//
// Created by Jamie Briggs on 10/07/2025.
//

#ifndef READING_H
#define READING_H
#include <chrono>
#include <vector>

struct Reading
{
    double level;
    std::chrono::system_clock::time_point timestamp;
};
struct Readings
{
    std::vector<Reading> readings;
};

#endif //READING_H
