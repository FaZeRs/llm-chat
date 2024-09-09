#include "backend.h"

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>

namespace llm_chat {

ChatBackend::ChatBackend(QObject *parent)
    : QObject(parent),
      m_Manager(new QNetworkAccessManager(this)),
      m_Thread(new ChatThread(this)) {}

ChatThread *ChatBackend::thread() const { return m_Thread.get(); }

void ChatBackend::sendMessage(const QString &message) {
  m_Thread->addMessage(message, true, {});
  sendRequestToOllama(message);
}

void ChatBackend::sendRequestToOllama(const QString &prompt) {
  QNetworkRequest request(QUrl("http://localhost:11434/api/generate"));
  request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

  // Get all the messages from the model that were sent by AI
  QJsonArray context;
  for (const Message *message : m_Thread->messages()) {
    if (!message->isUser()) {
      const auto message_context = message->context();
      for (const auto &context_item : message_context) {
        context.append(QJsonValue::fromVariant(context_item));
      }
    }
  }

  QJsonObject json;
  json["model"] = m_ModelName;
  json["prompt"] = prompt;
  json["stream"] = true;
  json["context"] = context;

  QNetworkReply *reply = m_Manager->post(request, QJsonDocument(json).toJson());

  m_CurrentMessageIndex = m_Thread->rowCount();
  m_Thread->addMessage("", false, {});

  connect(reply, &QNetworkReply::readyRead, this,
          [this, reply]() { handleStreamResponse(reply); });

  connect(reply, &QNetworkReply::finished, this, [this, reply]() {
    if (reply->error() != QNetworkReply::NoError) {
      QString errorMessage = "Error: " + reply->errorString();
      m_Thread->updateMessageText(m_CurrentMessageIndex, errorMessage);
    }
    reply->deleteLater();
  });
}

void ChatBackend::handleStreamResponse(QNetworkReply *reply) {
  QByteArray data = reply->readAll();
  QStringList lines = QString(data).split("\n", Qt::SkipEmptyParts);

  for (const QString &line : lines) {
    QJsonDocument json_response = QJsonDocument::fromJson(line.toUtf8());
    if (const auto content = json_response.object()["response"].toString();
        !content.isEmpty()) {
      m_Thread->updateMessageText(m_CurrentMessageIndex, content);
    }
    if (const auto context =
            json_response.object()["context"].toArray().toVariantList();
        !context.isEmpty()) {
      m_Thread->updateMessageContext(m_CurrentMessageIndex, context);
    }
  }
}

void ChatBackend::clearMessages() { m_Thread->clearMessages(); }

QString ChatBackend::modelName() const { return m_ModelName; }

void ChatBackend::setModelName(const QString &model_name) {
  m_ModelName = model_name;
}

void ChatBackend::fetchModelList() {
  QNetworkRequest request(
      QUrl("http://localhost:11434/api/tags"));  // Replace with actual endpoint
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

}  // namespace llm_chat