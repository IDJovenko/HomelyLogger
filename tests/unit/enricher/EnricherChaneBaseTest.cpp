#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "BaseEnricher.hpp"
#include "EnricherChainBase.hpp"
#include "MockEnricher.hpp"

namespace homelylog {
namespace {
class EnrichImplCallTracker : public EnricherChainBase {
 public:
  using EnricherChainBase::EnricherChainBase;

  bool enrichImplCalled = false;

 protected:
  void enrichImpl(LogRecord&) override { enrichImplCalled = true; }
};

class OrderTrackingEnricher : public EnricherChainBase {
 public:
  OrderTrackingEnricher(std::vector<std::string>& order, std::string name,
                        std::unique_ptr<ILogEnricher> next = nullptr)
      : EnricherChainBase(std::move(next)),
        order_(order),
        name_(std::move(name)) {}

 protected:
  void enrichImpl(LogRecord&) override { order_.push_back(name_); }

 private:
  std::vector<std::string>& order_;
  std::string name_;
};

class ThrowingOnEnrichEnricher : public EnricherChainBase {
 public:
  using EnricherChainBase::EnricherChainBase;

 protected:
  void enrichImpl(LogRecord&) override {
    throw std::runtime_error("enrichImpl failed");
  }
};

class RecordOverwritingEnricher : public EnricherChainBase {
 public:
  using EnricherChainBase::EnricherChainBase;
  static constexpr std::string_view newMessage = "new message";
  static constexpr std::string_view newLogger = "new logger";

 protected:
  void enrichImpl(LogRecord& record) override {
    record.message = newMessage;
    record.loggerName = newLogger;
  }
};

}  // namespace

//------------------------------------------------------------------------------
// Constructor Tests
//------------------------------------------------------------------------------

TEST(EnricherChainBase, Constructor_WithValidNext_CallsNextOnEnrich) {
  auto mock = std::make_unique<MockEnricher>();
  EXPECT_CALL(*mock, enrich(testing::_)).Times(1);

  EnrichImplCallTracker enricher(std::move(mock));

  LogRecord record{};

  EXPECT_NO_THROW(enricher.enrich(record));
  EXPECT_TRUE(enricher.enrichImplCalled);
}

TEST(EnricherChainBase, Constructor_WithNullptr_WorksAsChainTerminator) {
  EnrichImplCallTracker enricher(nullptr);

  LogRecord record{};

  EXPECT_NO_THROW(enricher.enrich(record));
  EXPECT_TRUE(enricher.enrichImplCalled);
}

TEST(EnricherChainBase, Constructor_WithDefault_WorksAsChainTerminator) {
  EnrichImplCallTracker enricher;

  LogRecord record{};

  EXPECT_NO_THROW(enricher.enrich(record));
  EXPECT_TRUE(enricher.enrichImplCalled);
}

//------------------------------------------------------------------------------
// enrich() Tests
//------------------------------------------------------------------------------

TEST(EnricherChainBase, Enrich_WithNext_CallsNextBeforeImpl) {
  std::vector<std::string> order;

  auto mock = std::make_unique<MockEnricher>();

  EXPECT_CALL(*mock, enrich(testing::_))
      .WillOnce(
          testing::Invoke([&](LogRecord&) { order.emplace_back("next"); }));

  OrderTrackingEnricher enricher(order, "impl", std::move(mock));

  LogRecord record{};

  enricher.enrich(record);

  std::vector<std::string> expectedOrder{"next", "impl"};
  EXPECT_EQ(order, expectedOrder);
}

TEST(EnricherChainBase, Enrich_WithLongChain_CallsInReverseOrder) {
  std::vector<std::string> order;

  auto mock = std::make_unique<MockEnricher>();

  EXPECT_CALL(*mock, enrich(testing::_))
      .WillOnce(
          testing::Invoke([&](LogRecord&) { order.emplace_back("mock"); }));

  auto enricher3 = std::make_unique<OrderTrackingEnricher>(order, "enricher3",
                                                           std::move(mock));

  auto enricher2 = std::make_unique<OrderTrackingEnricher>(
      order, "enricher2", std::move(enricher3));

  auto enricher1 = std::make_unique<OrderTrackingEnricher>(
      order, "enricher1", std::move(enricher2));

  LogRecord record{};

  enricher1->enrich(record);

  EXPECT_EQ(order, (std::vector<std::string>{"mock", "enricher3", "enricher2",
                                             "enricher1"}));
}

TEST(EnricherChainBase, Enrich_WithoutNext_WorksAsChainTerminator) {
  EnrichImplCallTracker enricher;

  LogRecord record{};

  EXPECT_NO_THROW(enricher.enrich(record));

  EXPECT_TRUE(enricher.enrichImplCalled);
}

TEST(EnricherChainBase, Enrich_WithOverwriting_AppliesAllChanges) {
  RecordOverwritingEnricher enricher;

  LogRecord record{};
  record.message = "old message";
  record.loggerName = "old logger";

  enricher.enrich(record);

  EXPECT_EQ(record.message, RecordOverwritingEnricher::newMessage);
  EXPECT_EQ(record.loggerName, RecordOverwritingEnricher::newLogger);
}

TEST(EnricherChainBase, Enrich_WhenNextThrows_PropagatesExceptionAndSkipsImpl) {
  auto mock = std::make_unique<MockEnricher>();

  EXPECT_CALL(*mock, enrich(testing::_))
      .WillOnce(testing::Throw(std::runtime_error("next failed")));

  EnrichImplCallTracker enricher(std::move(mock));

  LogRecord record{};

  EXPECT_THROW(enricher.enrich(record), std::runtime_error);
  EXPECT_FALSE(enricher.enrichImplCalled);
}

TEST(EnricherChainBase, Enrich_WhenImplThrows_PropagatesException) {
  auto mock = std::make_unique<MockEnricher>();

  EXPECT_CALL(*mock, enrich(testing::_)).Times(1);

  ThrowingOnEnrichEnricher enricher(std::move(mock));

  LogRecord record{};

  EXPECT_THROW(enricher.enrich(record), std::runtime_error);
}
}  // namespace homelylog
