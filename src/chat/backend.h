#pragma once

#include <qtmetamacros.h>

#include <QJsonArray>
#include <QNetworkAccessManager>
#include <QObject>

#include "thread.h"

namespace llm_chat {

/// @brief The ChatBackend class handles the communication with the Ollama
/// server.
class ChatBackend : public QObject {
  Q_OBJECT
  Q_PROPERTY(ChatThread *thread READ thread CONSTANT FINAL)
  Q_PROPERTY(QString modelName READ modelName WRITE setModelName)
  Q_PROPERTY(QStringList modelList READ modelList NOTIFY modelListFetched)

 public:
  /// @brief Constructs a new ChatBackend object.
  /// @param parent The parent object.
  explicit ChatBackend(QObject *parent = nullptr);
  /// @brief Returns the thread that holds the chat messages.
  /// @return The chat thread.
  [[nodiscard]] ChatThread *thread() const;
  /// @brief Get the name of the model.
  /// @return The name of the model as a QString.
  [[nodiscard]] QString modelName() const;
  /// @brief Get the list of available models.
  [[nodiscard]] QStringList modelList() const { return m_ModelList; }

 public slots:
  /// @brief Sends a message to the Ollama server.
  /// @param message The message to send.
  void sendMessage(const QString &message);
  /// @brief Clears the chat messages.
  void clearMessages();
  /// @brief Sets the model name.
  ///@details This function sets the name of the model to be used in the
  /// backend.
  /// @param modelName The name of the model.
  void setModelName(const QString &model_name);
  /// @brief Fetches the list of available models from the Ollama server.
  void fetchModelList();

 signals:
  /// @brief Emitted when the list of models is fetched.
  void modelListFetched();

 private:
  QScopedPointer<QNetworkAccessManager> m_Manager;
  QScopedPointer<ChatThread> m_Thread;
  int m_CurrentMessageIndex;
  QString m_ModelName{"llama3.1"};
  QList<QString> m_ModelList;

  /// @brief Sends a request to the Ollama server.
  /// @param prompt The prompt to send.
  void sendRequestToOllama(const QString &prompt);
  /// @brief Handles the response from the Ollama server.
  /// @param reply The response from the Ollama server.
  void handleStreamResponse(QNetworkReply *reply);
};

}  // namespace llm_chat
