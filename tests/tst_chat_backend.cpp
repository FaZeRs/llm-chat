#include <backend.h>

#include <QSettings>
#include <catch2/catch_test_macros.hpp>

class TestChatBackend : public llm_chat::ChatBackend {
 public:
  using llm_chat::ChatBackend::ChatBackend;

  llm_chat::ThreadList* getThreadList() { return threadList(); }
  llm_chat::ThreadProxyList* getProxyList() { return threadProxyList(); }
};

TEST_CASE("ChatBackend basic functionality", "[chat]") {
  TestChatBackend backend;

  SECTION("Initial state") {
    QSettings settings;
    settings.clear();
    qDebug() << backend.model();
    CHECK(backend.model().isEmpty());
    CHECK(backend.modelList().isEmpty());
    CHECK(backend.getThreadList() != nullptr);
    CHECK(backend.getProxyList() != nullptr);
  }

  SECTION("Model management") {
    const QString test_model = "llama2";
    backend.setModel(test_model);
    CHECK(backend.model() == test_model);
  }

  SECTION("Thread management") {
    const int initial_thread_count = backend.getThreadList()->rowCount();

    backend.sendMessage(0, "Test message");

    CHECK(backend.getThreadList()->rowCount() == initial_thread_count + 1);

    backend.deleteThread(0);
    CHECK(backend.getThreadList()->rowCount() == initial_thread_count);

    backend.sendMessage(0, "Test message 1");
    backend.sendMessage(1, "Test message 2");
    backend.clearThreads();
    CHECK(backend.getThreadList()->rowCount() == 0);
  }
}

TEST_CASE("ChatBackend thread access", "[chat]") {
  TestChatBackend backend;

  SECTION("Get thread by index") {
    backend.sendMessage(0, "Test message");

    llm_chat::Thread* thread = backend.getThread(0);
    CHECK(thread != nullptr);

    llm_chat::Thread* invalid_thread = backend.getThread(999);
    CHECK(invalid_thread == nullptr);
  }
}
