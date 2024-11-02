#pragma once

#include <QNetworkAccessManager>
#include <QObject>
#include <QSettings>

#include "thread.h"
#include "thread_list.h"
#include "thread_proxy_list.h"

namespace llm_chat {

/// @brief The ChatBackend class handles the communication with the Ollama
/// server.
class ChatBackend : public QObject {
  Q_OBJECT
  Q_PROPERTY(ThreadProxyList *sortedThreads READ threadProxyList CONSTANT FINAL)
  Q_PROPERTY(QString model READ model WRITE setModel NOTIFY modelChanged)
  Q_PROPERTY(QStringList modelList READ modelList NOTIFY modelListFetched)
  Q_PROPERTY(QString systemPrompt READ systemPrompt WRITE setSystemPrompt NOTIFY
                 systemPromptChanged)
  Q_PROPERTY(QString ollamaServerUrl READ ollamaServerUrl WRITE
                 setOllamaServerUrl NOTIFY ollamaServerUrlChanged)

 public:
  /// @brief Constructs a new ChatBackend object.
  /// @param parent The parent object.
  explicit ChatBackend(QObject *parent = nullptr);
  /// @brief Returns the chat threads.
  /// @return All the chat threads.
  [[nodiscard]] ThreadList *threadList() const { return m_ThreadList.get(); }
  /// @brief Returns the sorted chat threads.
  /// @return The sorted chat threads.
  [[nodiscard]] ThreadProxyList *threadProxyList() const {
    return m_ThreadProxyList.get();
  }
  /// @brief Get the name of the model.
  /// @return The name of the model.
  [[nodiscard]] QString model() const;
  /// @brief Get the list of available models.
  [[nodiscard]] QStringList modelList() const { return m_ModelList; }
  /// @brief Get the system prompt.
  /// @return The system prompt .
  [[nodiscard]] QString systemPrompt() const;
  /// @brief Get ollama server url.
  /// @return The ollama server url.
  [[nodiscard]] QString ollamaServerUrl() const;

 public Q_SLOTS:
  /// @brief Sets the model name.
  ///@details This function sets the name of the model to be used in the
  /// backend.
  /// @param model The name of the model.
  void setModel(const QString &model);
  /// @brief Fetches the list of available models from the Ollama server.
  void fetchModelList();
  /// @brief Returns the thread at the given index.
  /// @param index The index of the thread in proxy model.
  /// @return The thread at the given index.
  Thread *getThread(const int index);
  /// @brief Removes the thread at the given index.
  /// @param index The index of the thread in proxy model.
  void deleteThread(const int index);
  /// @brief Removes all the threads.
  void clearThreads();
  /// @brief Sends a message to the Ollama server.
  /// @param prompt The message to send.
  void sendMessage(const int index, const QString &prompt);
  /// @brief Set the system prompt.
  /// @param prompt The system prompt to set.
  void setSystemPrompt(const QString &prompt);
  /// @brief Set the ollama server url.
  /// @param url The ollama server url to set.
  void setOllamaServerUrl(const QString &url);
  /// @brief Retries the latest message.
  /// @param index The index of the thread in proxy model.
  void retryLatestMessage(const int index);

 signals:
  /// @brief Emitted when the list of models is fetched.
  void modelListFetched();
  /// @brief Emitted when the model is changed.
  void modelChanged();
  /// @brief Emitted when the current thread is changed.
  void newThreadCreated();
  /// @brief Emitted when the system prompt is changed.
  void systemPromptChanged();
  /// @brief Emitted when the ollama server url is changed.
  void ollamaServerUrlChanged();

 private:
  QScopedPointer<QNetworkAccessManager> m_Manager{new QNetworkAccessManager};
  QScopedPointer<ThreadList> m_ThreadList{new ThreadList};
  QScopedPointer<ThreadProxyList> m_ThreadProxyList{new ThreadProxyList};
  QList<QString> m_ModelList;
  QSettings m_Settings;

  /// @brief Sends a request to the Ollama server.
  /// @param prompt The prompt to send.
  void sendRequestToOllama(Thread *thread, const QString &prompt);
  /// @brief Handles the response from the Ollama server.
  /// @param thread The thread to update.
  /// @param reply The response from the Ollama server.
  void handleStreamResponse(Thread *thread, QNetworkReply *reply) const;
};

}  // namespace llm_chat
