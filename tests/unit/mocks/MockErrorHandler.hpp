#pragma once

#include <gmock/gmock.h>

#include "IErrorHandler.hpp"

namespace homelylog {

class MockErrorHandler : public IErrorHandler {
 public:
  MOCK_METHOD(void, onError, (std::string_view errorMsg), (override));
};

}  // namespace homelylog