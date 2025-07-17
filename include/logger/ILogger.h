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
  void debug(const String& msg)
  {
    this->debug(msg.c_str());
  }
  void debug(const long msg)
  {
    debug(String(msg));
  }
  virtual void info(const char*) = 0;
  void info(const String& msg)
  {
    this->info(msg.c_str());
  }
  void info(const long msg)
  {
    info(String(msg));
  }
  virtual void warning(const char*) = 0;
  virtual void error(const char*) = 0;
  void error(const String& msg)
  {
    this->error(msg.c_str());
  }
  void error(const int msg)
  {
    this->error(String(msg));
  }
  virtual void setLogLevel(LogLevel level) = 0;
};

#endif // ILOGGER_H
