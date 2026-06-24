#pragma once
#include <chrono>
#include <map>
#include <string>

#include "LogLevel.hpp"
namespace homelylog {
struct LogRecord {
  LogLevel level;
  std::string message;
  std::string loggerName;
  std::chrono::system_clock::time_point timestamp;
  std::map<std::string, std::string> fields;
};
}  // namespace homelylog
