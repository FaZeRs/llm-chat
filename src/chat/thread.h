#pragma once

#include <QAbstractListModel>
#include <QDateTime>
#include <QSortFilterProxyModel>
#include <QVector>

namespace llm_chat {

class Message;

/// @brief The Thread class provides a model for the chat interface.
class Thread : public QAbstractListModel {
  Q_OBJECT
  Q_PROPERTY(QList<Message *> messages READ messages CONSTANT FINAL)
  Q_PROPERTY(QDateTime createdAt READ createdAt CONSTANT FINAL)

 public:
  /// @brief The data roles for the model.
  enum MessageRoles { TextRole = Qt::UserRole + 1, IsUserRole, ContextRole };

  /// @brief Constructs a new Thread object.
  /// @param parent The parent object.
  explicit Thread(QObject *parent = nullptr);

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

  /// @brief Removes the latest message from the model.
  void removeLatestMessage();

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

}  // namespace llm_chat
