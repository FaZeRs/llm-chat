#include "backend.h"

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
#include <ranges>

#include "message.h"

namespace llm_chat {

static constexpr auto OLLAMA_HOST = "http://localhost:11434";

ChatBackend::ChatBackend(QObject *parent) : QObject(parent) {
  fetchModelList();

  m_ThreadProxyList->setSourceModel(m_ThreadList.get());
  m_Manager->connectToHost(ollamaServerUrl());
}

void ChatBackend::sendMessage(const int index, const QString &prompt) {
  auto [thread, is_new_thread] = [&]() -> std::pair<Thread *, bool> {
    if (index >= 0) {
      if (auto existing_thread =
              m_ThreadList->getThread(m_ThreadProxyList->mapToSource(
                  m_ThreadProxyList->index(index, 0)))) {
        return {existing_thread, false};
      }
    }
    return {m_ThreadList->createNewThread(), true};
  }();

  if (!thread->messages().empty()) {
    const auto &message = thread->messages().last();
    if (message && message->inProgress()) {
      return;
    }
  }

  thread->addMessage(prompt, true);
  sendRequestToOllama(thread, prompt);
  if (is_new_thread) emit newThreadCreated();
}

void ChatBackend::sendRequestToOllama(Thread *thread, const QString &prompt) {
  if (!thread) {
    qWarning() << "Thread is null";
    return;
  }

  auto request =
      QNetworkRequest(QUrl(QString("%1/api/generate").arg(ollamaServerUrl())));
  request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
  request.setAttribute(QNetworkRequest::Http2AllowedAttribute, true);
  request.setAttribute(QNetworkRequest::HttpPipeliningAllowedAttribute, true);

  const auto messages = thread->messages();
  auto context = QJsonArray::fromVariantList(
      messages |
      std::views::filter([](const auto &msg) { return !msg->isUser(); }) |
      std::views::transform([](const auto &msg) { return msg->context(); }) |
      std::views::join | std::ranges::to<QVariantList>());

  const auto &current_model = model();
  QJsonObject json{{"model", current_model.isEmpty() ? m_ModelList.constFirst()
                                                     : current_model},
                   {"prompt", prompt},
                   {"stream", true},
                   {"context", context}};

  if (const auto &system_prompt = systemPrompt(); !system_prompt.isEmpty()) {
    json["system"] = system_prompt;
  }

  auto *reply = m_Manager->post(
      request, QJsonDocument(json).toJson(QJsonDocument::Compact));

  thread->addMessage({}, false, {}, true);

  connect(reply, &QNetworkReply::readyRead, this,
          [this, thread, reply]() { handleStreamResponse(thread, reply); });

  connect(reply, &QNetworkReply::errorOccurred, this,
          [thread, reply](QNetworkReply::NetworkError error) {
            auto &message = thread->messages().last();
            if (message) {
              message->setText("Error: " + reply->errorString() + ", Code: " +
                               QString::number(static_cast<int>(error)));
              const auto msg_index = thread->index(thread->rowCount() - 1, 0);
              emit thread->dataChanged(msg_index, msg_index,
                                       {Thread::TextRole});
            }
          });

  connect(reply, &QNetworkReply::finished, this, [thread, reply]() {
    auto &message = thread->messages().last();
    if (message) {
      message->setInProgress(false);
      const auto msg_index = thread->index(thread->rowCount() - 1, 0);
      emit thread->dataChanged(msg_index, msg_index, {Thread::FinishedRole});
    }
    reply->deleteLater();
  });
}

void ChatBackend::handleStreamResponse(Thread *thread,
                                       QNetworkReply *reply) const {
  if (!thread) return;

  auto &message = thread->messages().last();
  if (!message) return;

  const auto data = reply->readAll();
  const auto lines =
      QString::fromUtf8(QByteArrayView{data}).split("\n", Qt::SkipEmptyParts);

  for (const auto &line : lines) {
    QJsonParseError error;
    if (const auto json_response =
            QJsonDocument::fromJson(line.toUtf8(), &error);
        error.error == QJsonParseError::NoError) {
      message->updateFromJson(json_response.object());
      const auto msg_index = thread->index(thread->rowCount() - 1, 0);
      emit thread->dataChanged(msg_index, msg_index,
                               {Thread::TextRole, Thread::ContextRole});
    } else {
      qWarning() << "Error: " << error.errorString() << ", Message: " << line;
    }
  }
}

QString ChatBackend::model() const {
  return m_Settings.value("model", "").toString();
}

void ChatBackend::setModel(const QString &model) {
  if (model == m_Settings.value("model").toString()) return;

  m_Settings.setValue("model", model);
  emit modelChanged();
}

void ChatBackend::fetchModelList() {
  QNetworkRequest request(QUrl(QString("%1/api/tags").arg(ollamaServerUrl())));
  QNetworkReply *reply = m_Manager->get(request);
  connect(reply, &QNetworkReply::finished, this, [this, reply]() {
    if (reply->error() == QNetworkReply::NoError) {
      const auto json_response = QJsonDocument::fromJson(reply->readAll());
      const auto json_array = json_response.object()["models"].toArray();
      QStringList model_list;
      for (const auto &value : json_array) {
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
  return m_Settings.value("system_prompt", "").toString();
}

void ChatBackend::setSystemPrompt(const QString &prompt) {
  if (prompt == m_Settings.value("system_prompt").toString()) return;

  m_Settings.setValue("system_prompt", prompt);
  emit systemPromptChanged();
}

QString ChatBackend::ollamaServerUrl() const {
  return m_Settings.value("ollama_host", OLLAMA_HOST).toString();
}

void ChatBackend::setOllamaServerUrl(const QString &url) {
  if (url == m_Settings.value("ollama_server_url").toString()) return;

  m_Settings.setValue("ollama_server_url", url);
  m_Manager->connectToHost(url);
  emit ollamaServerUrlChanged();
}

void ChatBackend::retryLatestMessage(const int index) {
  auto thread = getThread(index);
  if (!thread || thread->messages().isEmpty()) return;

  const auto messages = thread->messages();
  QString last_user_message;
  for (auto it = messages.rbegin(); it != messages.rend(); ++it) {
    if ((*it)->isUser()) {
      last_user_message = (*it)->text();
      break;
    }
  }

  if (last_user_message.isEmpty()) return;

  thread->removeLatestMessage();
  sendRequestToOllama(thread, last_user_message);
}
}  // namespace llm_chat
