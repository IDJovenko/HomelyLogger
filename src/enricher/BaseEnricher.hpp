#pragma once
#include "ILogEnricher.hpp"

namespace homelylog {
// Does nothing, simply returns the record as is.
// This is the bottom of the chain.
// It's needed so that other enrichers can always unconditionally call the next
// one — no need to check whether the next one exists or not.
class BaseEnricher : public ILogEnricher {
 public:
  ~BaseEnricher() noexcept override = default;
  void enrich(LogRecord& /*record*/) override {
    // Do nothing
  }
};
}  // namespace homelylog
