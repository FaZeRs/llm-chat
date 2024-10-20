#include <message.h>

#include <catch2/catch_test_macros.hpp>

TEST_CASE("Test constructor", "[Message]") {
  llm_chat::Message message("Hello", true, {QVariant("context")});
  REQUIRE(message.text() == "Hello");
  REQUIRE(message.isUser() == true);
  REQUIRE(message.context() == QVector<QVariant>({QVariant("context")}));
}

TEST_CASE("Test set text", "[Message]") {
  llm_chat::Message message("Hello", true, {});
  message.setText("World");
  REQUIRE(message.text() == "World");
}

TEST_CASE("Test set context", "[Message]") {
  llm_chat::Message message("Hello", true, {});
  message.setContext({QVariant("context")});
  REQUIRE(message.context() == QVector<QVariant>({QVariant("context")}));
}
