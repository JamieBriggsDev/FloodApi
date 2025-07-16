//
// Created by Jamie Briggs on 16/07/2025.
//

#ifndef ILOGGER_H
#define ILOGGER_H

enum class LogLevel
{
  DEBUG,
  INFO,
  WARNING,
  ERROR
};

class ILogger
{
  public:
  virtual ~ILogger() = default;

  virtual void debug(const char*) = 0;
  virtual void info(const char*) = 0;
  virtual void warning(const char*) = 0;
  virtual void error(const char*) = 0;
  virtual void setLogLevel(LogLevel level) = 0;
};

#endif // ILOGGER_H
