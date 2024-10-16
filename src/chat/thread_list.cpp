#include "thread_list.h"

#include "message.h"
#include "thread.h"

namespace llm_chat {

ThreadList::ThreadList(QObject *parent) : QAbstractListModel(parent) {}

int ThreadList::rowCount(const QModelIndex &parent) const {
  if (parent.isValid()) return 0;
  return static_cast<int>(m_Threads.size());
}

QVariant ThreadList::data(const QModelIndex &index, int role) const {
  if (!index.isValid()) return {};

  const Thread *thread = m_Threads.at(index.row());
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

QHash<int, QByteArray> ThreadList::roleNames() const {
  QHash<int, QByteArray> roles;
  roles[TextRole] = "text";
  roles[CreatedAtRole] = "createdAt";
  return roles;
}

Thread *ThreadList::createNewThread() {
  const auto new_thread = new Thread(this);
  beginInsertRows(QModelIndex(), rowCount(), rowCount());
  m_Threads.append(new_thread);
  endInsertRows();
  return new_thread;
}

void ThreadList::deleteThread(const QModelIndex &model_index) {
  if (!model_index.isValid()) return;
  const auto index = model_index.row();
  beginRemoveRows(QModelIndex(), index, index);
  delete m_Threads.takeAt(index);
  endRemoveRows();
}

void ThreadList::deleteAllThreads() {
  beginResetModel();
  qDeleteAll(m_Threads);
  m_Threads.clear();
  endResetModel();
}

}  // namespace llm_chat
