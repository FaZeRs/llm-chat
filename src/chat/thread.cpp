#include "thread.h"

#include <QJsonArray>
#include <QJsonObject>

#include "message.h"

namespace llm_chat {

Thread::Thread(QObject *parent) : QAbstractListModel(parent) {
  m_CreatedAt = QDateTime::currentDateTime();
}

int Thread::rowCount(const QModelIndex &parent) const {
  if (parent.isValid()) return 0;
  return static_cast<int>(m_Messages.size());
}

QVariant Thread::data(const QModelIndex &index, int role) const {
  if (!index.isValid()) return {};

  const Message *message = m_Messages.at(index.row());
  switch (role) {
    case TextRole:
      return message->text();
    case IsUserRole:
      return message->isUser();
    case ContextRole:
      return message->context();
    default:
      return {};
  }
}

QHash<int, QByteArray> Thread::roleNames() const {
  QHash<int, QByteArray> roles;
  roles[TextRole] = "text";
  roles[IsUserRole] = "isUser";
  roles[ContextRole] = "context";
  return roles;
}

void Thread::addMessage(const QString &text, bool isUser,
                        const QVector<QVariant> &context) {
  beginInsertRows(QModelIndex(), rowCount(), rowCount());
  m_Messages.append(new Message(text, isUser, context, this));
  endInsertRows();
}

void Thread::updateMessageText(const qsizetype index, const QString &new_text) {
  if (index >= 0 && index < m_Messages.size()) {
    const auto text = m_Messages[index]->text() + new_text;
    m_Messages[index]->setText(text);
    QModelIndex model_index = this->index(static_cast<int>(index), 0);
    emit dataChanged(model_index, model_index, {TextRole});
  }
}

void Thread::updateMessageContext(const qsizetype index,
                                  const QVector<QVariant> &context) {
  if (index >= 0 && index < m_Messages.size()) {
    m_Messages[index]->setContext(context);
  }
}

void Thread::clearMessages() {
  beginResetModel();
  qDeleteAll(m_Messages);
  m_Messages.clear();
  endResetModel();
}

void Thread::updateLatestMessage(const QString &text) {
  if (!m_Messages.empty()) {
    updateMessageText(m_Messages.size() - 1, text);
  }
}

void Thread::updateLatestMessage(const QJsonObject &json) {
  if (m_Messages.empty()) {
    return;
  }
  if (const auto content = json["response"].toString(); !content.isEmpty()) {
    updateMessageText(m_Messages.size() - 1, content);
  }
  if (const auto context = json["context"].toArray().toVariantList();
      !context.isEmpty()) {
    updateMessageContext(m_Messages.size() - 1, context);
  }
}

}  // namespace llm_chat
