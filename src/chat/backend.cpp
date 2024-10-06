#include "backend.h"

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
#include <QSettings>

#include "thread.h"

namespace llm_chat {

ChatBackend::ChatBackend(QObject *parent)
    : QObject(parent), m_Manager(new QNetworkAccessManager(this)) {
  fetchModelList();

  m_Threads = new ChatThreadsModel(this);
  m_SortedThreads = new SortedChatThreadsModel(this);
  m_SortedThreads->setSourceModel(m_Threads);
}

void ChatBackend::sendMessage(const int index, const QString &message) {
  auto curr_thread = thread(index);
  bool new_thread = false;
  if (!curr_thread) {
    curr_thread = m_Threads->createNewThread();
    new_thread = true;
  }
  curr_thread->addMessage(message, true, {});
  sendRequestToOllama(curr_thread, message);
  if (new_thread) {
    const auto source_index = m_Threads->index(m_Threads->rowCount() - 1, 0);
    const auto proxy_index = m_SortedThreads->mapFromSource(source_index);
    emit currentThreadChanged(proxy_index.row());
  }
}

void ChatBackend::sendRequestToOllama(ChatThread *thread,
                                      const QString &prompt) {
  if (!thread) {
    qWarning() << "Thread is null";
    return;
  }
  QNetworkRequest request(QUrl("http://localhost:11434/api/generate"));
  request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

  // Get all the messages from the model that were sent by AI
  QJsonArray context;
  for (const Message *message : thread->messages()) {
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

void ChatBackend::handleStreamResponse(ChatThread *thread,
                                       QNetworkReply *reply) {
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

ChatThread *ChatBackend::thread(const int index) const {
  if (index < 0) return nullptr;

  const auto proxy_index = m_SortedThreads->index(index, 0);
  const auto source_index = m_SortedThreads->mapToSource(proxy_index);
  const auto &threads = m_Threads->threads();
  if (source_index.row() < 0 || source_index.row() >= threads.size()) {
    return nullptr;
  }
  return m_Threads->threads()[source_index.row()];
}

void ChatBackend::deleteThread(const int index) {
  if (index < 0) return;
  const auto proxy_index = m_SortedThreads->index(index, 0);
  const auto source_index = m_SortedThreads->mapToSource(proxy_index);
  m_Threads->deleteThread(source_index);
}

void ChatBackend::clearThreads() { m_Threads->deleteAllThreads(); }

}  // namespace llm_chat
