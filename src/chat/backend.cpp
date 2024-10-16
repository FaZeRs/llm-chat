#include "backend.h"

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
#include <QSettings>

#include "message.h"
#include "thread.h"
#include "thread_list.h"
#include "thread_proxy_list.h"

namespace llm_chat {

ChatBackend::ChatBackend(QObject *parent)
    : QObject(parent),
      m_Manager(new QNetworkAccessManager(this)),
      m_ThreadList(new ThreadList(this)),
      m_ThreadProxyList(new ThreadProxyList(this)) {
  fetchModelList();

  m_ThreadProxyList->setSourceModel(m_ThreadList.get());

  connect(this, &ChatBackend::sendMessage, this, &ChatBackend::onSendMessage,
          Qt::QueuedConnection);
}

void ChatBackend::onSendMessage(const QString &message) {
  if (!m_OpenedThread) {
    m_OpenedThread = m_ThreadList->createNewThread();
    emit openedThreadChanged();
  }
  m_OpenedThread->addMessage(message, true, {});
  sendRequestToOllama(m_OpenedThread, message);
}

void ChatBackend::sendRequestToOllama(Thread *thread, const QString &prompt) {
  if (!thread) {
    qWarning() << "Thread is null";
    return;
  }
  QNetworkRequest request(QUrl("http://localhost:11434/api/generate"));
  request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

  // Get all the messages from the model that were sent by AI
  QJsonArray context;
  for (const auto &message : thread->messages()) {
    if (!message->isUser()) {
      const auto message_context = message->context();
      for (const auto &context_item : message_context) {
        context.append(QJsonValue::fromVariant(context_item));
      }
    }
  }

  QJsonObject json;
  json["model"] = model();
  json["prompt"] = prompt;
  json["stream"] = true;
  json["context"] = context;

  QNetworkReply *reply = m_Manager->post(request, QJsonDocument(json).toJson());

  thread->addMessage("", false, {});

  connect(reply, &QNetworkReply::readyRead, this,
          [this, thread, reply]() { handleStreamResponse(thread, reply); });

  connect(reply, &QNetworkReply::finished, this, [thread, reply]() {
    if (reply->error() != QNetworkReply::NoError) {
      QString errorMessage = "Error: " + reply->errorString();
      thread->updateLatestMessage(errorMessage);
    }
    reply->deleteLater();
  });
}

void ChatBackend::handleStreamResponse(Thread *thread, QNetworkReply *reply) {
  if (!thread) return;
  QByteArray data = reply->readAll();
  QStringList lines = QString(data).split("\n", Qt::SkipEmptyParts);

  for (const QString &line : lines) {
    QJsonDocument json_response = QJsonDocument::fromJson(line.toUtf8());
    thread->updateLatestMessage(json_response.object());
  }
}

QString ChatBackend::model() const {
  QSettings settings;
  return settings.value("model", "").toString();
}

void ChatBackend::setModel(const QString &model) {
  QSettings settings;
  if (model == settings.value("model").toString()) return;

  settings.setValue("model", model);
  emit modelChanged();
}

void ChatBackend::fetchModelList() {
  QNetworkRequest request(QUrl("http://localhost:11434/api/tags"));
  QNetworkReply *reply = m_Manager->get(request);
  connect(reply, &QNetworkReply::finished, this, [this, reply]() {
    if (reply->error() == QNetworkReply::NoError) {
      QJsonDocument json_response = QJsonDocument::fromJson(reply->readAll());
      QJsonArray json_array = json_response.object()["models"].toArray();
      QStringList model_list;
      for (const QJsonValue &value : json_array) {
        model_list.append(value.toObject()["name"].toString());
      }
      m_ModelList = model_list;
      emit modelListFetched();
    }
    reply->deleteLater();
  });
}

[[nodiscard]] Thread *ChatBackend::openedThread() const {
  return m_OpenedThread;
}

void ChatBackend::setOpenedThread(Thread *new_opened_thread) {
  if (m_OpenedThread == new_opened_thread) return;
  m_OpenedThread = new_opened_thread;
  emit openedThreadChanged();
}

// ChatThread *ChatBackend::thread(const int index) const {
//   if (index < 0) return nullptr;

//   const auto proxy_index = m_SortedThreads->index(index, 0);
//   const auto source_index = m_SortedThreads->mapToSource(proxy_index);
//   const auto &threads = m_Threads->threads();
//   if (source_index.row() < 0 || source_index.row() >= threads.size()) {
//     return nullptr;
//   }
//   return m_Threads->threads()[source_index.row()];
// }

// void ChatBackend::deleteThread(const int index) {
//   if (index < 0) return;
//   const auto proxy_index = m_SortedThreads->index(index, 0);
//   const auto source_index = m_SortedThreads->mapToSource(proxy_index);
//   m_Threads->deleteThread(source_index);
// }

// void ChatBackend::clearThreads() { m_Threads->deleteAllThreads(); }

}  // namespace llm_chat
