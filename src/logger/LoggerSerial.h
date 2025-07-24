//
// Created by Jamie Briggs on 16/07/2025.
//

#ifndef LOGGER_H
#define LOGGER_H

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
    log(Flood::LogLevel::F_DEBUG, message);
  }

  void info(const char* message) override {
    log(Flood::LogLevel::F_INFO, message);
  }

  void warning(const char* message) override {
    log(Flood::LogLevel::F_WARNING, message);
  }

  void error(const char* message) override {
    log(Flood::LogLevel::F_ERROR, message);
  }

  void setLogLevel(Flood::LogLevel level) override
  {
    minimumLogLevel_ = level;
  }

  private:
  LoggerSerial() = default;

  void log(Flood::LogLevel level, const char* message)
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

    const char* levelStr;
    switch (level)
    {
    case Flood::LogLevel::F_DEBUG:
      levelStr = "DEBUG";
      break;
    case Flood::LogLevel::F_INFO:
      levelStr = "INFO";
      break;
    case Flood::LogLevel::F_WARNING:
      levelStr = "WARNING";
      break;
    case Flood::LogLevel::F_ERROR:
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

  Flood::LogLevel minimumLogLevel_ = Flood::LogLevel::F_DEBUG;
};


#endif

#endif // LOGGER_H
