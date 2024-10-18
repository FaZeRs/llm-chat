#include "backend.h"

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
#include <QSettings>

#include "message.h"

namespace llm_chat {

ChatBackend::ChatBackend(QObject *parent) : QObject(parent) {
  fetchModelList();

  m_ThreadProxyList->setSourceModel(m_ThreadList.get());

  connect(this, &ChatBackend::sendMessage, this, &ChatBackend::onSendMessage,
          Qt::QueuedConnection);
}

void ChatBackend::onSendMessage(const int index, const QString &message) {
  Thread *thread = nullptr;
  if (index >= 0) {
    const auto proxy_index = m_ThreadProxyList->index(index, 0);
    const auto source_index = m_ThreadProxyList->mapToSource(proxy_index);
    thread = m_ThreadList->getThread(source_index);
    thread->addMessage(message, true, {});
  } else {
    thread = m_ThreadList->createNewThread();
    thread->addMessage(message, true, {});
    emit newThreadCreated();
  }
  sendRequestToOllama(thread, message);
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

Thread *ChatBackend::getThread(const int index) {
  if (index < 0) return nullptr;
  const auto proxy_index = m_ThreadProxyList->index(index, 0);
  const auto source_index = m_ThreadProxyList->mapToSource(proxy_index);
  return m_ThreadList->getThread(source_index);
}

void ChatBackend::deleteThread(const int index) {
  if (index < 0) return;
  const auto proxy_index = m_ThreadProxyList->index(index, 0);
  const auto source_index = m_ThreadProxyList->mapToSource(proxy_index);
  m_ThreadList->deleteThread(source_index);
}

void ChatBackend::clearThreads() {
  m_ThreadList->deleteAllThreads();
}

}  // namespace llm_chat
