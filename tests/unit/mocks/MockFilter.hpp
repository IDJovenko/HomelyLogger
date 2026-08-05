#pragma once

#include <gmock/gmock.h>

#include "ILogFilter.hpp"

namespace homelylog {

class MockFilter : public ILogFilter {
 public:
  MOCK_METHOD(bool, shouldLog, (const LogRecord& record), (const, override));
};

}  // namespace homelylog