#pragma once
#include "LogRecord.hpp"

namespace homelylog {
class ILogFilter {
 public:
  virtual ~ILogFilter() noexcept = default;
  virtual bool shouldLog(const LogRecord& record) const = 0;
};
}  // namespace homelylog