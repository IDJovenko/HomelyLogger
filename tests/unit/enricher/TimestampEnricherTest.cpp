#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <chrono>

#include "MockEnricher.hpp"
#include "TimestampEnricher.hpp"

namespace homelylog {

namespace {
class MessageEnricher : public EnricherChainBase {
 public:
  using EnricherChainBase::EnricherChainBase;
  static constexpr std::string_view newMessage = "test message";

 protected:
  void enrichImpl(LogRecord& record) override { record.message = newMessage; }
};
}  // namespace

//------------------------------------------------------------------------------
// Constructor Tests
//------------------------------------------------------------------------------

TEST(TimestampEnricher, Constructor_WithValidNext_CallsNextOnEnrich) {
  auto mock = std::make_unique<MockEnricher>();

  EXPECT_CALL(*mock, enrich(testing::_)).Times(1);

  TimestampEnricher enricher(std::move(mock));

  LogRecord record{};

  EXPECT_NO_THROW(enricher.enrich(record));
}

//------------------------------------------------------------------------------
// enrich() Tests
//------------------------------------------------------------------------------

TEST(TimestampEnricher, Enrich_OnAnyRecord_SetsTimestampToCurrentTime) {
  TimestampEnricher enricher;

  LogRecord record{};

  const auto before = std::chrono::system_clock::now();

  enricher.enrich(record);

  const auto after = std::chrono::system_clock::now();

  EXPECT_GE(record.timestamp, before);
  EXPECT_LE(record.timestamp, after);
}

TEST(TimestampEnricher, Enrich_WithCompleteRecord_OnlyModifiesTimestamp) {
  TimestampEnricher enricher;

  LogRecord record{};
  record.message = "original message";
  record.loggerName = "logger";
  record.level = LogLevel::INFO;

  const auto originalMessage = record.message;
  const auto originalLogger = record.loggerName;
  const auto originalLevel = record.level;

  enricher.enrich(record);

  EXPECT_EQ(record.message, originalMessage);
  EXPECT_EQ(record.loggerName, originalLogger);
  EXPECT_EQ(record.level, originalLevel);

  EXPECT_NE(record.timestamp, std::chrono::system_clock::time_point{});
}

TEST(TimestampEnricher, Enrich_InChain_PreservesOtherEnrichersChanges) {
  auto messageEnricher = std::make_unique<MessageEnricher>();

  TimestampEnricher enricher(std::move(messageEnricher));

  LogRecord record{};

  enricher.enrich(record);

  EXPECT_EQ(record.message, MessageEnricher::newMessage);
  EXPECT_NE(record.timestamp, std::chrono::system_clock::time_point{});
}

TEST(TimestampEnricher, Enrich_WithExistingTimestamp_OverwritesIt) {
  TimestampEnricher enricher;

  LogRecord record{};
  record.timestamp =
      std::chrono::system_clock::time_point{std::chrono::seconds{1}};

  const auto oldTimestamp = record.timestamp;

  enricher.enrich(record);

  EXPECT_NE(record.timestamp, oldTimestamp);
  EXPECT_GT(record.timestamp, oldTimestamp);
}

}  // namespace homelylog
