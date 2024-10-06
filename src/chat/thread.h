#pragma once

#include <QAbstractListModel>
#include <QDateTime>
#include <QSortFilterProxyModel>
#include <QVector>

#include "message.h"

namespace llm_chat {

/// @brief The ChatThread class provides a model for the chat interface.
class ChatThread : public QAbstractListModel {
  Q_OBJECT
  Q_PROPERTY(QList<Message *> messages READ messages CONSTANT)
  Q_PROPERTY(QDateTime createdAt READ createdAt CONSTANT)

 public:
  /// @brief The data roles for the model.
  enum MessageRoles { TextRole = Qt::UserRole + 1, IsUserRole, ContextRole };

  /// @brief Constructs a new ChatThread object.
  /// @param parent The parent object.
  explicit ChatThread(QObject *parent = nullptr);

  /// @brief Returns the number of rows under the given parent.
  /// @param parent The parent index.
  [[nodiscard]] int rowCount(
      const QModelIndex &parent = QModelIndex()) const override;
  /// @brief Returns the data stored under the given role for the item referred
  /// to by the index.
  /// @param index The model index.
  /// @param role The data role.
  [[nodiscard]] QVariant data(const QModelIndex &index,
                              int role = Qt::DisplayRole) const override;
  /// @brief Returns the role names for the model.
  /// @return The role names for the model.
  [[nodiscard]] QHash<int, QByteArray> roleNames() const override;
  /// @brief Adds a new message to the model.
  /// @param text The message text.
  /// @param is_user Whether the message is from the user.
  /// @param context The message context.
  Q_INVOKABLE void addMessage(const QString &text, bool is_user,
                              const QVector<QVariant> &context);

  /// @brief Returns the list of messages.
  /// @return The list of messages.
  [[nodiscard]] inline const QList<Message *> &messages() const {
    return m_Messages;
  }
  /// @brief Clears the list of messages.
  void clearMessages();

  /// @brief Update latest message text
  /// @param text The message text.
  void updateLatestMessage(const QString &text);

  /// @brief Update latest message from json
  /// @param json The json object.
  void updateLatestMessage(const QJsonObject &json);

  /// @brief Returns the creation time of the chat thread.
  /// @return The creation time of the chat thread.
  [[nodiscard]] inline const QDateTime &createdAt() const {
    return m_CreatedAt;
  }

 private:
  QList<Message *> m_Messages;
  QDateTime m_CreatedAt;

  /// @brief Updates the text of a message.
  /// @param index The index of the message.
  /// @param new_text The new text of the message.
  void updateMessageText(const qsizetype index, const QString &new_text);
  /// @brief Updates the context of a message.
  /// @param index The index of the message.
  /// @param new_context The new context of the message.
  void updateMessageContext(const qsizetype index,
                            const QVector<QVariant> &new_context);
};

class ChatThreadsModel : public QAbstractListModel {
  Q_OBJECT
  Q_PROPERTY(QList<ChatThread *> threads READ threads CONSTANT)

 public:
  /// @brief The data roles for the model.
  enum ThreadRoles { TextRole = Qt::UserRole + 1, CreatedAtRole };

  /// @brief Constructs a new ChatThread object.
  /// @param parent The parent object.
  explicit ChatThreadsModel(QObject *parent = nullptr);

  /// @brief Returns the number of rows under the given parent.
  /// @param parent The parent index.
  [[nodiscard]] int rowCount(
      const QModelIndex &parent = QModelIndex()) const override;
  /// @brief Returns the data stored under the given role for the item referred
  /// to by the index.
  /// @param index The model index.
  /// @param role The data role.
  [[nodiscard]] QVariant data(const QModelIndex &index,
                              int role = Qt::DisplayRole) const override;
  /// @brief Returns the role names for the model.
  /// @return The role names for the model.
  [[nodiscard]] QHash<int, QByteArray> roleNames() const override;

  /// @brief Returns the list of threads.
  /// @return The list of threads.
  [[nodiscard]] inline const QList<ChatThread *> &threads() const {
    return m_Threads;
  }
  /// @brief Creates a new thread.
  /// @details This function creates a new thread and sets it as the active
  /// thread.
  ChatThread *createNewThread();
  /// @brief Removes the thread at the given index.
  /// @param index The index of the thread to remove.
  void deleteThread(const QModelIndex &model_index);
  /// @brief Removes all the threads.
  void deleteAllThreads();

 private:
  QList<ChatThread *> m_Threads;
};

/// @brief The SortedChatThreadsModel class provides a sorted model for the chat
class SortedChatThreadsModel : public QSortFilterProxyModel {
  Q_OBJECT

 public:
  /// @brief Constructs a new SortedChatThreadsModel object.
  /// @param parent The parent object.
  explicit SortedChatThreadsModel(QObject *parent = nullptr);

 protected:
  /// @brief Returns true if the left item is less than the right item.
  /// @param left The left model index.
  /// @param right The right model index.
  /// @return True if the left item is less than the right item.
  [[nodiscard]] bool lessThan(const QModelIndex &left,
                              const QModelIndex &right) const override;
};

}  // namespace llm_chat
