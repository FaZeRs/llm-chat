#include <thread.h>

#include <QTest>

class TestChatThread : public QObject {
  Q_OBJECT

 private slots:
  void testAddMessage() const;
  void testUpdateMessageText() const;
  void testUpdateMessageContext() const;
  void testClearMessages() const;
};

void TestChatThread::testAddMessage() const {
  llm_chat::ChatThread thread;
  QCOMPARE(thread.rowCount(), 0);

  thread.addMessage("Hello", true, {});
  QCOMPARE(thread.rowCount(), 1);
  QCOMPARE(thread.data(thread.index(0, 0), llm_chat::ChatThread::TextRole)
               .toString(),
           "Hello");
  QCOMPARE(thread.data(thread.index(0, 0), llm_chat::ChatThread::IsUserRole)
               .toBool(),
           true);

  thread.addMessage("World", false, {QVariant("context")});
  QCOMPARE(thread.rowCount(), 2);
  QCOMPARE(thread.data(thread.index(1, 0), llm_chat::ChatThread::TextRole)
               .toString(),
           "World");
  QCOMPARE(thread.data(thread.index(1, 0), llm_chat::ChatThread::IsUserRole)
               .toBool(),
           false);
  QCOMPARE(thread.data(thread.index(1, 0), llm_chat::ChatThread::ContextRole)
               .toList(),
           QVariantList({"context"}));
}

void TestChatThread::testUpdateMessageText() const {
  llm_chat::ChatThread thread;
  thread.addMessage("Hello", true, {});

  thread.updateMessageText(0, " World");
  QCOMPARE(thread.data(thread.index(0, 0), llm_chat::ChatThread::TextRole)
               .toString(),
           "Hello World");
}

void TestChatThread::testUpdateMessageContext() const {
  llm_chat::ChatThread thread;
  thread.addMessage("Hello", true, {});

  thread.updateMessageContext(0, {QVariant("context")});
  QCOMPARE(thread.data(thread.index(0, 0), llm_chat::ChatThread::ContextRole)
               .toList(),
           QVariantList({"context"}));
}

void TestChatThread::testClearMessages() const {
  llm_chat::ChatThread thread;
  thread.addMessage("Hello", true, {});
  thread.addMessage("World", false, {});
  QCOMPARE(thread.rowCount(), 2);

  thread.clearMessages();
  QCOMPARE(thread.rowCount(), 0);
}

QTEST_MAIN(TestChatThread)
#include "tst_chat_thread.moc"