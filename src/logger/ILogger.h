//
// Created by Jamie Briggs on 16/07/2025.
//

#ifndef ILOGGER_H
#define ILOGGER_H

#if defined(ARDUINO)
#include <WString.h>
#endif

#include <cstdarg>
#include <cstdio>
#include <cstring>
#include <string>

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

#if defined(ARDUINO)
  // String overloads
  void debug(const String& msg) { this->debug(msg.c_str()); }

  void info(const String& msg) { this->info(msg.c_str()); }

  void warning(const String& msg) { this->warning(msg.c_str()); }

  void error(const String& msg) { this->error(msg.c_str()); }

  // std::string overloads
  void debug(const std::string msg) { this->debug(msg.c_str()); }

  void info(const std::string msg) { this->info(msg.c_str()); }

  void warning(const std::string msg) { this->warning(msg.c_str()); }

  void error(const std::string msg) { this->error(msg.c_str()); }
#endif

  // Long overloads
  void debug(const long msg) { debug(std::to_string(msg).c_str()); }

  void info(const long msg) { info(std::to_string(msg).c_str()); }

  void error(const int msg) { this->error(std::to_string(msg).c_str()); }
};

#endif // ILOGGER_H
