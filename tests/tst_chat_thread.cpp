#include <thread.h>

#include <QJsonArray>
#include <QJsonObject>
#include <QTest>
#include <catch2/catch_test_macros.hpp>

TEST_CASE("Test add message", "[Thread]") {
  llm_chat::Thread thread;
  REQUIRE(thread.createdAt().isValid());
  REQUIRE(thread.createdAt().isNull() == false);
  REQUIRE(thread.createdAt() == QDateTime::currentDateTime());

  REQUIRE(thread.rowCount() == 0);
  thread.addMessage("Hello", true, {});
  REQUIRE(thread.rowCount() == 1);
  REQUIRE(
      thread.data(thread.index(0, 0), llm_chat::Thread::TextRole).toString() ==
      "Hello");
  REQUIRE(
      thread.data(thread.index(0, 0), llm_chat::Thread::IsUserRole).toBool() ==
      true);
  REQUIRE(
      thread.data(thread.index(0, 0), llm_chat::Thread::IsUserRole).toBool() ==
      true);
  thread.addMessage("World", false, {QVariant("context")});
  REQUIRE(thread.rowCount() == 2);
  REQUIRE(
      thread.data(thread.index(1, 0), llm_chat::Thread::TextRole).toString() ==
      "World");
  REQUIRE(
      thread.data(thread.index(1, 0), llm_chat::Thread::IsUserRole).toBool() ==
      false);
  REQUIRE(
      thread.data(thread.index(1, 0), llm_chat::Thread::ContextRole).toList() ==
      QVariantList({"context"}));
}

TEST_CASE("Test clear messages", "[Thread]") {
  llm_chat::Thread thread;
  thread.addMessage("Hello", true);
  thread.addMessage("World", false);
  REQUIRE(thread.rowCount() == 2);
  thread.clearMessages();
  REQUIRE(thread.rowCount() == 0);
}
