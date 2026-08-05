#pragma once

#include <gmock/gmock.h>

#include "ILogSink.hpp"

namespace homelylog {

class MockLogSink : public ILogSink {
 public:
  MOCK_METHOD(void, write, (const LogRecord& record), (override));
  MOCK_METHOD(void, flush, (), (override));
  MOCK_METHOD(bool, open, (), (noexcept, override));
};

}  // namespace homelylog
