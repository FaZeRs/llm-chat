#include "thread.h"

namespace llm_chat {

ChatThread::ChatThread(QObject *parent) : QAbstractListModel(parent) {}

int ChatThread::rowCount(const QModelIndex &parent) const {
  if (parent.isValid()) return 0;
  return static_cast<int>(m_Messages.size());
}

QVariant ChatThread::data(const QModelIndex &index, int role) const {
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

QHash<int, QByteArray> ChatThread::roleNames() const {
  QHash<int, QByteArray> roles;
  roles[TextRole] = "text";
  roles[IsUserRole] = "isUser";
  roles[ContextRole] = "context";
  return roles;
}

void ChatThread::addMessage(const QString &text, bool isUser,
                            const QVector<QVariant> &context) {
  beginInsertRows(QModelIndex(), rowCount(), rowCount());
  m_Messages.append(new Message(text, isUser, context, this));
  endInsertRows();
}

void ChatThread::updateMessageText(const int index, const QString &new_text) {
  if (index >= 0 && index < m_Messages.size()) {
    const auto text = m_Messages[index]->text() + new_text;
    m_Messages[index]->setText(text);
    QModelIndex model_index = this->index(index, 0);
    emit dataChanged(model_index, model_index, {TextRole});
  }
}

void ChatThread::updateMessageContext(const int index,
                                      const QVector<QVariant> &context) {
  if (index >= 0 && index < m_Messages.size()) {
    m_Messages[index]->setContext(context);
  }
}

void ChatThread::clearMessages() {
  beginResetModel();
  qDeleteAll(m_Messages);
  m_Messages.clear();
  endResetModel();
}

}  // namespace llm_chat