#pragma once

#include <string_view>
#include <system_error>

namespace homelylog {
class IErrorHandler {
 public:
  virtual ~IErrorHandler() = default;

  virtual void onError(std::string_view errorMsg) = 0;
};
}  // namespace homelylog
