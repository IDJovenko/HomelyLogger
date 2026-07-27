#pragma once
#include "LogRecord.hpp"
#include "functional"

namespace homelylog {

class ILogSink {
 public:
  virtual ~ILogSink() noexcept = default;

  virtual void write(const LogRecord& record) = 0;

  virtual void flush() = 0;

  // Checks if the sink has been opened.
  virtual bool open() noexcept = 0;  // TODO in the current API
                                     // (without open()-close() methods)
                                     // it seems to be always open.
                                     // Perhaps this method should be removed?
};
}  // namespace homelylog
