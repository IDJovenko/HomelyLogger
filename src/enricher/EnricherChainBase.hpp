#pragma once

#include "BaseEnricher.hpp"
#include "ILogEnricher.hpp"

namespace homelylog {

// Base class for the enricher chain
//
// Implements the "Chain of Responsibility" pattern for sequentially
// enriching log records. Each enricher in the chain passes it on to the next
// one and adds its information to the record.
//
// Example of assembling a chain of 2 enrichers:
// auto chain = std::make_unique<MyCustomEnricher>(
//                 std::make_unique<TimestampEnricher>()
//             );
// WARNING: The chain processing logic does not check for the existence of
// the next enricher; therefore, a `BaseEnricher` that performs no action must
// be placed at the end of the chain.
class EnricherChainBase : public ILogEnricher {
 public:
  // The constructor accepts the next enricher in the chain
  // If the next enricher is not specified, BaseEnricher is placed at
  // the end of chain.
  explicit EnricherChainBase(std::unique_ptr<ILogEnricher> next = nullptr)
      : next_(next ? std::move(next) : std::make_unique<BaseEnricher>()) {}
  EnricherChainBase(const EnricherChainBase&) = delete;
  EnricherChainBase& operator=(const EnricherChainBase&) = delete;
  // Move operations are deleted to guarantee next_ is never nullptr.
  // Moved-from object would violate this invariant and cause UB on enrich().
  // Since chains are created once and never moved, this restriction is fine.
  EnricherChainBase(EnricherChainBase&&) = delete;
  EnricherChainBase& operator=(EnricherChainBase&&) = delete;

  void enrich(LogRecord& record) final {
    // 1. Pass the record to the next one in the chain
    next_->enrich(record);  // there is no need to check for nullptr, because
                            // the last enricher in the chain always is a
                            // BaseEnricher, which does nothing.

    // 2. Add your own field
    enrichImpl(record);
  }

 protected:
  // Method that inheritors must override
  // Adds a specific field to a record
  virtual void enrichImpl(LogRecord& record) = 0;

 private:
  std::unique_ptr<ILogEnricher> next_;
};
}  // namespace homelylog
