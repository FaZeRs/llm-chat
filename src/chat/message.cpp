#include "message.h"

#include <QJsonArray>

namespace llm_chat {

Message::Message(const QString text, const bool is_user,
                 const QVector<QVariant> &context, const bool in_progress,
                 QObject *parent)
    : QObject(parent),
      m_Text(text),
      m_IsUser(is_user),
      m_Context(context),
      m_InProgress(in_progress) {}

QString Message::text() const { return m_Text; }

void Message::setText(const QString text) { m_Text = text; }

bool Message::isUser() const { return m_IsUser; }

QVector<QVariant> Message::context() const { return m_Context; }

void Message::setContext(const QVector<QVariant> &context) {
  m_Context = context;
}

bool Message::inProgress() const { return m_InProgress; }

void Message::setInProgress(const bool in_progress) {
  m_InProgress = in_progress;
}

void Message::updateFromJson(const QJsonObject &json) {
  if (const auto content = json["response"].toString(); !content.isEmpty()) {
    m_Text = m_Text + content;
  }
  if (const auto context = json["context"].toArray().toVariantList();
      !context.isEmpty()) {
    m_Context = context;
  }
}

}  // namespace llm_chat
