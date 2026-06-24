#pragma once

#include <string>
#include <unordered_map>

namespace homelylog {

// Defining X-macro for logging levels
#define LOG_LEVELS \
  X(TRACE)         \
  X(DEBUG)         \
  X(INFO)          \
  X(WARN)          \
  X(ERROR)         \
  X(FATAL)

enum class LogLevel {
#define X(name) name,
  LOG_LEVELS
#undef X
};

// clang-format off
inline std::string_view toString(LogLevel level) {
  switch (level) {
    #define X(name) case LogLevel::name: return #name;
    LOG_LEVELS
    #undef X
  default:
    return "UNKNOWN";
  }
}
// clang-format on

#undef LOG_LEVELS

}  // namespace homelylog