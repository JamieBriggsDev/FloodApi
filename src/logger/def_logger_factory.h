//
// Created by Jamie Briggs on 16/07/2025.
//

#ifndef LOGFACTORY_H
#define LOGFACTORY_H

#if defined(ARDUINO) || defined(ESP32)

#include "LoggerSerial.h"
#define LOG jbriggs::common::logger::LoggerSerial::getInstance()

#else

#include "Logger.h"
#define LOG jbriggs::common::logger::Logger::getInstance()

#endif

#endif //LOGFACTORY_H
