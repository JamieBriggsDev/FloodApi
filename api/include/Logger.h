//
// Created by Jamie Briggs on 16/07/2025.
//

#ifndef LOGGER_H
#define LOGGER_H

#include <mutex>
#include <string>

#if defined(ARDUINO) || defined(ESP32)
#include <Arduino.h>
#else
#include <ctime>
#include <iostream>
#endif

enum class LogLevel
{
    DEBUG,
    INFO,
    WARNING,
    ERROR
};

class Logger
{
public:
    static Logger& getInstance()
    {
        static Logger instance;
        return instance;
    }

    // Delete copy constructor and assignment operator
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

    template <typename T>
    void debug(const T& message)
    {
        log(LogLevel::DEBUG, message);
    }

    template <typename T>
    void info(const T& message)
    {
        log(LogLevel::INFO, message);
    }

    template <typename T>
    void warning(const T& message)
    {
        log(LogLevel::WARNING, message);
    }

    template <typename T>
    void error(const T& message)
    {
        log(LogLevel::ERROR, message);
    }

    void setLogLevel(LogLevel level)
    {
        std::lock_guard<std::mutex> lock(mutex_);
        minimumLogLevel_ = level;
    }

private:
    Logger() = default;

    template <typename T>
    void log(LogLevel level, const T& message)
    {
        if (level < minimumLogLevel_)
            return;

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

#if defined(ARDUINO) || defined(ESP32)
        Serial.print("[");
        Serial.print(levelStr);
        Serial.print("] ");
        Serial.println(message);
#else
        std::time_t now = std::time(nullptr);
        char timeStr[20];
        std::strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S", std::localtime(&now));
        std::cout << timeStr << " [" << levelStr << "] " << message << std::endl;
#endif
    }

    LogLevel minimumLogLevel_ = LogLevel::DEBUG;
    std::mutex mutex_;
};

// Convenience macro for easy access
#define LOG Logger::getInstance()


#endif // LOGGER_H
