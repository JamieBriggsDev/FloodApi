//
// Created by Jamie Briggs on 16/07/2025.
//

#ifndef ILOGGER_H
#define ILOGGER_H
#include <WString.h>
#include <cstdarg>
#include <stdio.h>

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

  // Format string methods
  void debug_f(const char* format, ...)
  {
    char buffer[256];
    va_list args;
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);
    debug(buffer);
  }

  void info_f(const char* format, ...)
  {
    char buffer[256];
    va_list args;
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);
    info(buffer);
  }

  void warning_f(const char* format, ...)
  {
    char buffer[256];
    va_list args;
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);
    warning(buffer);
  }

  void error_f(const char* format, ...)
  {
    char buffer[256];
    va_list args;
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);
    error(buffer);
  }

  // String overloads
  void debug(const String& msg)
  {
    this->debug(msg.c_str());
  }

  void info(const String& msg)
  {
    this->info(msg.c_str());
  }

  void warning(const String& msg)
  {
    this->warning(msg.c_str());
  }

  void error(const String& msg)
  {
    this->error(msg.c_str());
  }

  // std::string overloads
  void debug(const std::string msg)
  {
    this->debug(msg.c_str());
  }

  void info(const std::string msg)
  {
    this->info(msg.c_str());
  }

  void warning(const std::string msg)
  {
    this->warning(msg.c_str());
  }

  void error(const std::string msg)
  {
    this->error(msg.c_str());
  }

  // Long overloads
  void debug(const long msg)
  {
    debug(String(msg));
  }

  void info(const long msg)
  {
    info(String(msg));
  }

  void error(const int msg)
  {
    this->error(String(msg));
  }


};

#endif // ILOGGER_H
