#pragma once

#include <map>
#include <memory>
#include <string>
#include <string_view>

#include "IErrorHandler.hpp"
#include "ILogEnricher.hpp"
#include "ILogFilter.hpp"
#include "ILogSink.hpp"

namespace homelylog {

class ILogger {
 public:
  virtual ~ILogger() = default;

  // Logs a message with the specified level and additional fields
  //   level   — logging level (e.g., LogLevel::Info, LogLevel::Error)
  //   message — message to log.
  //   fields  — arbitrary key-value pairs
  //          that the source wants to attach to the record, e.g.
  //          {{"method", "GET"}, {"path", "/api"}, {"status", "200"}}
  //
  // The method processes the message through the following pipeline:
  // 1. Filters the message (if a filter is set)
  // 2. Enriches the message with additional context (if an enricher is set)
  // 3. Passes the final message to all registered sinks
  //
  //  This method provides the following exception guarantees:
  //  - **Internal errors** (errors during message enrichment and filtering,
  //  errors writing to receivers, formatting problems, I/O errors):
  //  All exceptions that occur during processing of a message are caught
  //  within the method and passed to the error handler.
  //  After handling the error, the method returns normally -
  //  exceptions **are not thrown** externally.
  //  - **Custom error handler**: If a custom error handler is installed for
  //  the logger (via setErrorHandler()), and this handler throws an
  //  exception, the exception is **not caught** within the method and will
  //  be passed on to the calling code. This is done intentionally to allow
  //  applications to implement their own strategy for handling critical
  //  errors (e.g., crashes, restarts, etc.).
  virtual void log(LogLevel lvl, std::string message,
                   std::map<std::string, std::string> fields = {}) = 0;

  virtual void flush() = 0;

  virtual void setFilter(std::unique_ptr<ILogFilter> filter) = 0;
  virtual void setEnricher(std::unique_ptr<ILogEnricher> enricher) = 0;
  virtual void addSink(std::shared_ptr<ILogSink> sink) = 0;
  virtual void removeSink(std::shared_ptr<ILogSink> sink) = 0;
  virtual void setErrorHandler(std::unique_ptr<IErrorHandler> errorHandler) = 0;

  // Returns the name of the logger
  virtual const std::string& getName() const = 0;
};
}  // namespace homelylog
