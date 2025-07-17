//
// Created by Jamie Briggs on 17/07/2025.
//

#ifndef DATE_UTIL_H
#define DATE_UTIL_H

#include <chrono>
#include <iomanip>
#include <sstream>

// Convert string date ("2022-12-25") to time_point
inline std::chrono::system_clock::time_point parseDate(const std::string& dateStr) {
  std::tm tm = {};
  std::istringstream ss(dateStr);
  ss >> std::get_time(&tm, "%Y-%m-%d");
  return std::chrono::system_clock::from_time_t(std::mktime(&tm));
}

// Convert time_point to string date ("2022-12-25")
inline std::string formatDate(const std::chrono::system_clock::time_point& tp) {
  auto time = std::chrono::system_clock::to_time_t(tp);
  std::stringstream ss;
  ss << std::put_time(std::localtime(&time), "%Y-%m-%d");
  return ss.str();
}


#endif //DATE_UTIL_H
