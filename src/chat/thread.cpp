#include "thread.h"

#include <QJsonArray>
#include <QJsonObject>

namespace llm_chat {

ChatThread::ChatThread(QObject *parent) : QAbstractListModel(parent) {
  m_CreatedAt = QDateTime::currentDateTime();
}

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

void ChatThread::updateMessageText(const qsizetype index,
                                   const QString &new_text) {
  if (index >= 0 && index < m_Messages.size()) {
    const auto text = m_Messages[index]->text() + new_text;
    m_Messages[index]->setText(text);
    QModelIndex model_index = this->index(static_cast<int>(index), 0);
    emit dataChanged(model_index, model_index, {TextRole});
  }
}

void ChatThread::updateMessageContext(const qsizetype index,
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

void ChatThread::updateLatestMessage(const QString &text) {
  if (m_Messages.size() > 0) {
    updateMessageText(m_Messages.size() - 1, text);
  }
}

void ChatThread::updateLatestMessage(const QJsonObject &json) {
  if (m_Messages.size() > 0) {
    if (const auto content = json["response"].toString(); !content.isEmpty()) {
      updateMessageText(m_Messages.size() - 1, content);
    }
    if (const auto context = json["context"].toArray().toVariantList();
        !context.isEmpty()) {
      updateMessageContext(m_Messages.size() - 1, context);
    }
  }
}

ChatThreadsModel::ChatThreadsModel(QObject *parent)
    : QAbstractListModel(parent) {}

int ChatThreadsModel::rowCount(const QModelIndex &parent) const {
  if (parent.isValid()) return 0;
  return static_cast<int>(m_Threads.size());
}

QVariant ChatThreadsModel::data(const QModelIndex &index, int role) const {
  if (!index.isValid()) return {};

  const ChatThread *thread = m_Threads.at(index.row());
  switch (role) {
    case TextRole:
      return thread->messages().isEmpty() ? ""
                                          : thread->messages().front()->text();
    case CreatedAtRole:
      return thread->createdAt();
    default:
      return {};
  }
}

QHash<int, QByteArray> ChatThreadsModel::roleNames() const {
  QHash<int, QByteArray> roles;
  roles[TextRole] = "text";
  roles[CreatedAtRole] = "createdAt";
  return roles;
}

ChatThread *ChatThreadsModel::createNewThread() {
  const auto new_thread = new ChatThread(this);
  beginInsertRows(QModelIndex(), rowCount(), rowCount());
  m_Threads.append(new_thread);
  endInsertRows();
  return new_thread;
}

void ChatThreadsModel::deleteThread(const QModelIndex &model_index) {
  if (!model_index.isValid()) return;
  const auto index = model_index.row();
  beginRemoveRows(QModelIndex(), index, index);
  delete m_Threads.takeAt(index);
  endRemoveRows();
}

void ChatThreadsModel::deleteAllThreads() {
  beginResetModel();
  qDeleteAll(m_Threads);
  m_Threads.clear();
  endResetModel();
}

SortedChatThreadsModel::SortedChatThreadsModel(QObject *parent)
    : QSortFilterProxyModel(parent) {
  setDynamicSortFilter(true);
  setSortRole(ChatThreadsModel::CreatedAtRole);
  sort(0, Qt::AscendingOrder);
}

bool SortedChatThreadsModel::lessThan(const QModelIndex &left,
                                      const QModelIndex &right) const {
  QVariant left_data =
      sourceModel()->data(left, ChatThreadsModel::CreatedAtRole);
  QVariant right_data =
      sourceModel()->data(right, ChatThreadsModel::CreatedAtRole);

  QDateTime left_date_time = left_data.toDateTime();
  QDateTime right_date_time = right_data.toDateTime();

  return left_date_time > right_date_time;
}

}  // namespace llm_chat
