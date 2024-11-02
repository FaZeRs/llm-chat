#include "backend.h"

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
#include <QSettings>

#include "message.h"

namespace llm_chat {

constexpr auto OLLAMA_HOST = "http://localhost:11434";

ChatBackend::ChatBackend(QObject *parent) : QObject(parent) {
  fetchModelList();

  m_ThreadProxyList->setSourceModel(m_ThreadList.get());
}

void ChatBackend::sendMessage(const int index, const QString &message) {
  Thread *thread = nullptr;
  bool new_thread = false;
  if (index >= 0) {
    const auto proxy_index = m_ThreadProxyList->index(index, 0);
    const auto source_index = m_ThreadProxyList->mapToSource(proxy_index);
    thread = m_ThreadList->getThread(source_index);
    if (!thread) {
      thread = m_ThreadList->createNewThread();
      new_thread = true;
    }
    thread->addMessage(message, true, {});
  } else {
    thread = m_ThreadList->createNewThread();
    thread->addMessage(message, true, {});
    new_thread = true;
  }
  sendRequestToOllama(thread, message);
  if (new_thread) emit newThreadCreated();
}

void ChatBackend::sendRequestToOllama(Thread *thread, const QString &prompt) {
  if (!thread) {
    qWarning() << "Thread is null";
    return;
  }

  QNetworkRequest request(
      QUrl(QString("%1/api/generate").arg(ollamaServerUrl())));
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
  if (!systemPrompt().isEmpty()) {
    json["system"] = systemPrompt();
  }

  QNetworkReply *reply = m_Manager->post(request, QJsonDocument(json).toJson());

  thread->addMessage("", false, {});

  connect(reply, &QNetworkReply::readyRead, this,
          [this, thread, reply]() { handleStreamResponse(thread, reply); });

  connect(reply, &QNetworkReply::finished, this, [thread, reply]() {
    if (reply->error() != QNetworkReply::NoError) {
      qWarning()
          << "Error: " << reply->error()
          << ", Message: " << reply->errorString() << ", Code: "
          << reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
      const auto error_message = "Error: " + reply->errorString();
      thread->updateLatestMessage(error_message);
    }
    reply->deleteLater();
  });
}

void ChatBackend::handleStreamResponse(Thread *thread,
                                       QNetworkReply *reply) const {
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
  QNetworkRequest request(QUrl(QString("%1/api/tags").arg(ollamaServerUrl())));
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

void ChatBackend::clearThreads() { m_ThreadList->deleteAllThreads(); }

QString ChatBackend::systemPrompt() const {
  QSettings settings;
  return settings.value("system_prompt", "").toString();
}

void ChatBackend::setSystemPrompt(const QString &prompt) {
  QSettings settings;
  if (prompt == settings.value("system_prompt").toString()) return;

  settings.setValue("system_prompt", prompt);
  emit systemPromptChanged();
}

QString ChatBackend::ollamaServerUrl() const {
  QSettings settings;
  return settings.value("ollama_host", OLLAMA_HOST).toString();
}

void ChatBackend::setOllamaServerUrl(const QString &url) {
  QSettings settings;
  if (url == settings.value("ollama_server_url").toString()) return;

  settings.setValue("ollama_server_url", url);
  emit ollamaServerUrlChanged();
}

}  // namespace llm_chat
