#pragma once
#include "EnricherChainBase.hpp"

namespace homelylog {
// The enricher adds to the record it's creation time
// with at least millisecond precision.
class TimestampEnricher : public EnricherChainBase {
 public:
  explicit TimestampEnricher(std::unique_ptr<ILogEnricher> next = nullptr)
      : EnricherChainBase(std::move(next)) {}

 protected:
  void enrichImpl(LogRecord& record) override {
    record.timestamp = std::chrono::system_clock::now();
  }
};
}  // namespace homelylog
