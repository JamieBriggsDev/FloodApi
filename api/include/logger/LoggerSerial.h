//
// Created by Jamie Briggs on 16/07/2025.
//

#ifndef LOGGER_H
#define LOGGER_H

#include <mutex>

#include "ILogger.h"

#if defined(ARDUINO) || defined(ESP32)
#include <Arduino.h>


class LoggerSerial : public ILogger
{
public:
  static ILogger& getInstance()
  {
    static LoggerSerial instance;
    return instance;
  }

  // Delete copy constructor and assignment operator
  LoggerSerial(const LoggerSerial&) = delete;
  LoggerSerial& operator=(const LoggerSerial&) = delete;

  void debug(const char* message) override {
    log(LogLevel::DEBUG, message);
  }

  void info(const char* message) override {
    log(LogLevel::INFO, message);
  }

  void warning(const char* message) override {
    log(LogLevel::WARNING, message);
  }

  void error(const char* message) override {
    log(LogLevel::ERROR, message);
  }

  void setLogLevel(LogLevel level) override
  {
    std::lock_guard<std::mutex> lock(mutex_);
    minimumLogLevel_ = level;
  }

  private:
  LoggerSerial() = default;

  void log(LogLevel level, const char* message)
  {
    if (level < minimumLogLevel_)
      return;

    /** Lock mutex when constructed, to prevent race conditions when writing log messages
    Before:
      [DEBUG] First m[INFO] Second messaes
      sagege

    After:
      [DEBUG] First message
      [INFO] Second message
     */
    std::lock_guard<std::mutex> lock(mutex_);

    const char* levelStr;
    switch (level)
    {
    case LogLevel::DEBUG:
      levelStr = "DEBUG";
      break;
    case LogLevel::INFO:
      levelStr = "INFO";
      break;
    case LogLevel::WARNING:
      levelStr = "WARNING";
      break;
    case LogLevel::ERROR:
      levelStr = "ERROR";
      break;
    default:
      levelStr = "UNKNOWN";
    }

    Serial.print("[");
    Serial.print(levelStr);
    Serial.print("] ");
    Serial.println(message);
  }

  LogLevel minimumLogLevel_ = LogLevel::DEBUG;
  std::mutex mutex_;
};


#endif

#endif // LOGGER_H
