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
    case FinishedRole:
      return !message->inProgress();
    default:
      return {};
  }
}

QHash<int, QByteArray> Thread::roleNames() const {
  QHash<int, QByteArray> roles;
  roles[TextRole] = "text";
  roles[IsUserRole] = "isUser";
  roles[ContextRole] = "context";
  roles[FinishedRole] = "finished";
  return roles;
}

void Thread::addMessage(const QString &text, bool isUser,
                        const QVector<QVariant> &context,
                        const bool in_progress) {
  beginInsertRows(QModelIndex(), rowCount(), rowCount());
  m_Messages.append(new Message(text, isUser, context, in_progress, this));
  endInsertRows();
}

void Thread::clearMessages() {
  beginResetModel();
  qDeleteAll(m_Messages);
  m_Messages.clear();
  endResetModel();
}

void Thread::removeLatestMessage() {
  if (!m_Messages.empty()) {
    beginRemoveRows(QModelIndex(), rowCount() - 1, rowCount() - 1);
    delete m_Messages.takeLast();
    endRemoveRows();
  }
}

}  // namespace llm_chat
