#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "ILogEnricher.hpp"

namespace homelylog {

class MockEnricher : public ILogEnricher {
 public:
  MOCK_METHOD(void, enrich, (LogRecord&), (override));
};
}  // namespace homelylog