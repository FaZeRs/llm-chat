#pragma once

#include <QJsonObject>
#include <QObject>
#include <QString>
#include <QVariant>
#include <QVector>

namespace llm_chat {

/// @brief The Message class represents a chat message.
class Message : public QObject {
  Q_OBJECT
  Q_PROPERTY(QString text READ text CONSTANT FINAL)
  Q_PROPERTY(bool isUser READ isUser CONSTANT FINAL)
  Q_PROPERTY(QVector<QVariant> context READ context CONSTANT FINAL)
  Q_PROPERTY(bool inProgress READ inProgress CONSTANT FINAL)

 public:
  /// @brief Constructs a new Message object.
  /// @param text The text of the message.
  /// @param is_user True if the message was sent by the user, false otherwise.
  /// @param context The context of the message.
  /// @param in_progress Whether the message is in progress.
  /// @param parent The parent object.
  Message(const QString text, const bool is_user,
          const QVector<QVariant> &context, const bool in_progress = false,
          QObject *parent = nullptr);

  /// @brief Returns the text of the message.
  /// @return The text of the message.
  [[nodiscard]] QString text() const;
  /// @brief Sets the text of the message.
  /// @param text The new text of the message.
  void setText(const QString text);
  /// @brief Returns true if the message was sent by the user, false otherwise.
  /// @return True if the message was sent by the user, false otherwise.
  [[nodiscard]] bool isUser() const;
  /// @brief Returns the context of the message.
  /// @details This is used to provide additional information to the LLM.
  /// @return The context of the message.
  [[nodiscard]] QVector<QVariant> context() const;
  /// @brief Sets the context of the message.
  /// @param context The new context of the message.
  void setContext(const QVector<QVariant> &context);
  /// @brief Gets the in progress flag of the message.
  /// @return True if the message is in progress, false otherwise.
  [[nodiscard]] bool inProgress() const;
  /// @brief Sets the in progress flag of the message.
  /// @param in_progress The new in progress flag of the message.
  void setInProgress(const bool in_progress);
  /// @brief Updates the message from a JSON object.
  /// @param json The JSON object to update the message from.
  void updateFromJson(const QJsonObject &json);

 private:
  QString m_Text;
  bool m_IsUser;
  QVector<QVariant> m_Context;
  bool m_InProgress{false};
};

}  // namespace llm_chat

Q_DECLARE_METATYPE(llm_chat::Message *)
