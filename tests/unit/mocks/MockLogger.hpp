#pragma once

#include <gmock/gmock.h>

#include "ILogger.hpp"

namespace homelylog {

class MockLogger : public ILogger {
 public:
  MOCK_METHOD(void, log,
              (LogLevel lvl, std::string message,
               (std::map<std::string, std::string> fields)),
              (override));

  MOCK_METHOD(void, flush, (), (override));

  MOCK_METHOD(void, setFilter, (std::unique_ptr<ILogFilter> filter),
              (override));
  MOCK_METHOD(void, setEnricher, (std::unique_ptr<ILogEnricher> enricher),
              (override));
  MOCK_METHOD(void, addSink, (std::shared_ptr<ILogSink> sink), (override));
  MOCK_METHOD(void, removeSink, (std::shared_ptr<ILogSink> sink), (override));
  MOCK_METHOD(void, setErrorHandler,
              (std::unique_ptr<IErrorHandler> errorHandler), (override));

  MOCK_METHOD(const std::string&, getName, (), (const, override));
};

}  // namespace homelylog