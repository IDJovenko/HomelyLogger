#pragma once
#include "LogRecord.hpp"

namespace homelylog {
class ILogEnricher {
 public:
  virtual ~ILogEnricher() noexcept = default;
  virtual void enrich(LogRecord& record) = 0;
};
}  // namespace homelylog
