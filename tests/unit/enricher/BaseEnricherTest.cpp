#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "BaseEnricher.hpp"
#include "LogRecord.hpp"

namespace homelylog {

TEST(BaseEnricher, Enrich_OnAnyRecord_DoesNotModifyMessage) {
  BaseEnricher enricher;
  LogRecord record{};
  const std::string originalMessage = "test message";
  record.message = originalMessage;

  enricher.enrich(record);

  EXPECT_EQ(record.message, originalMessage);
}

TEST(BaseEnricher, Enrich_OnAnyRecord_DoesNotModifyLoggerName) {
  BaseEnricher enricher;
  LogRecord record{};
  const std::string originalLoggerName = "test.logger";
  record.loggerName = originalLoggerName;

  enricher.enrich(record);

  EXPECT_EQ(record.loggerName, originalLoggerName);
}

TEST(BaseEnricher, Enrich_OnAnyRecord_DoesNotModifyTimestamp) {
  BaseEnricher enricher;
  LogRecord record{};
  const auto originalTimestamp = std::chrono::system_clock::now();
  record.timestamp = originalTimestamp;

  enricher.enrich(record);

  EXPECT_EQ(record.timestamp, originalTimestamp);
}

TEST(BaseEnricher, Enrich_OnAnyRecord_DoesNotModifyLevel) {
  BaseEnricher enricher;
  LogRecord record{};
  const auto originalLevel = LogLevel::WARN;
  record.level = originalLevel;

  enricher.enrich(record);

  EXPECT_EQ(record.level, originalLevel);
}

TEST(BaseEnricher, Enrich_OnAnyRecord_DoesNotModifyAnyField) {
  BaseEnricher enricher;
  LogRecord record{};
  record.message = "message";
  record.loggerName = "logger";
  record.timestamp = std::chrono::system_clock::now();
  record.level = LogLevel::INFO;

  const auto originalRecord = record;  // Копируем для сравнения

  enricher.enrich(record);

  // Сравниваем все поля
  EXPECT_EQ(record.message, originalRecord.message);
  EXPECT_EQ(record.loggerName, originalRecord.loggerName);
  EXPECT_EQ(record.timestamp, originalRecord.timestamp);
  EXPECT_EQ(record.level, originalRecord.level);
}

TEST(BaseEnricher, Enrich_NeverThrows) {
  BaseEnricher enricher;
  LogRecord record{};

  EXPECT_NO_THROW(enricher.enrich(record));
  EXPECT_NO_THROW(enricher.enrich(record));
  EXPECT_NO_THROW(enricher.enrich(record));
}

TEST(BaseEnricher, Enrich_MultipleCalls_RecordRemainsUnchanged) {
  BaseEnricher enricher;
  LogRecord record{};
  record.message = "initial message";

  enricher.enrich(record);
  enricher.enrich(record);
  enricher.enrich(record);

  EXPECT_EQ(record.message, "initial message");
}

TEST(BaseEnricher, Enrich_WithEmptyRecord_NoSideEffects) {
  BaseEnricher enricher;
  LogRecord record{};  // Все поля по умолчанию

  EXPECT_NO_THROW(enricher.enrich(record));

  // Проверяем, что пустая запись осталась пустой
  EXPECT_TRUE(record.message.empty());
  EXPECT_TRUE(record.loggerName.empty());
  // timestamp и level будут иметь значения по умолчанию
}

TEST(BaseEnricher, Enrich_WithLargeRecord_NoSideEffects) {
  BaseEnricher enricher;
  LogRecord record{};
  const std::string largeMessage(1024 * 1024, 'A');  // 1 MB сообщение
  record.message = largeMessage;

  enricher.enrich(record);

  EXPECT_EQ(record.message, largeMessage);
}

}  // namespace homelylog