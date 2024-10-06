#pragma once

#include <QNetworkAccessManager>
#include <QObject>

#include "thread.h"

namespace llm_chat {

/// @brief The ChatBackend class handles the communication with the Ollama
/// server.
class ChatBackend : public QObject {
  Q_OBJECT
  Q_PROPERTY(
      SortedChatThreadsModel *sortedThreads READ sortedThreads CONSTANT FINAL)
  Q_PROPERTY(QString model READ model WRITE setModel NOTIFY modelChanged)
  Q_PROPERTY(QStringList modelList READ modelList NOTIFY modelListFetched)

 public:
  /// @brief Constructs a new ChatBackend object.
  /// @param parent The parent object.
  explicit ChatBackend(QObject *parent = nullptr);
  /// @brief Returns the chat threads.
  /// @return All the chat threads.
  [[nodiscard]] ChatThreadsModel *threads() const { return m_Threads; }

  /// @brief Returns the sorted chat threads.
  /// @return The sorted chat threads.
  [[nodiscard]] SortedChatThreadsModel *sortedThreads() const {
    return m_SortedThreads;
  }
  /// @brief Get the name of the model.
  /// @return The name of the model as a QString.
  [[nodiscard]] QString model() const;
  /// @brief Get the list of available models.
  [[nodiscard]] QStringList modelList() const { return m_ModelList; }

 public slots:
  /// @brief Sends a message to the Ollama server.
  /// @param index The index of the thread to send
  /// @param message The message to send.
  void sendMessage(const int index, const QString &message);
  /// @brief Sets the model name.
  ///@details This function sets the name of the model to be used in the
  /// backend.
  /// @param model The name of the model.
  void setModel(const QString &model);
  /// @brief Fetches the list of available models from the Ollama server.
  void fetchModelList();
  /// @brief Returns the active chat thread.
  /// @param index The index of the thread to get.
  /// @return The active chat thread. If no thread e
  [[nodiscard]] ChatThread *thread(const int index) const;
  /// @brief Removes the thread at the given index.
  /// @param index The index of the thread to remove.
  void deleteThread(const int index);
  /// @brief Clears all the threads
  void clearThreads();

 signals:
  /// @brief Emitted when the list of models is fetched.
  void modelListFetched();
  /// @brief Emitted when the model is changed.
  void modelChanged();
  /// @brief Emitted when the current thread is changed.
  void currentThreadChanged(const int index);

 private:
  QScopedPointer<QNetworkAccessManager> m_Manager;
  ChatThreadsModel *m_Threads;
  SortedChatThreadsModel *m_SortedThreads;
  QList<QString> m_ModelList;

  /// @brief Sends a request to the Ollama server.
  /// @param prompt The prompt to send.
  void sendRequestToOllama(ChatThread *thread, const QString &prompt);
  /// @brief Handles the response from the Ollama server.
  /// @param reply The response from the Ollama server.
  void handleStreamResponse(ChatThread *thread, QNetworkReply *reply);
};

}  // namespace llm_chat
