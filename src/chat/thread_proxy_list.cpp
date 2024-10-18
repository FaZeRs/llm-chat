#include "thread_proxy_list.h"

#include <QDateTime>

#include "thread_list.h"

namespace llm_chat {

ThreadProxyList::ThreadProxyList(QObject *parent)
    : QSortFilterProxyModel(parent) {
  setFilterRole(ThreadList::CreatedAtRole);
  setSortRole(ThreadList::CreatedAtRole);
  sort(0, Qt::AscendingOrder);
}

bool ThreadProxyList::lessThan(const QModelIndex &left,
                               const QModelIndex &right) const {
  QVariant left_data = sourceModel()->data(left, ThreadList::CreatedAtRole);
  QVariant right_data = sourceModel()->data(right, ThreadList::CreatedAtRole);

  QDateTime left_date_time = left_data.toDateTime();
  QDateTime right_date_time = right_data.toDateTime();

  return left_date_time > right_date_time;
}

// Thread *ThreadProxyList::get(const int index) const {
//   if (index < 0 || index >= rowCount()) return nullptr;

//   const auto model_index = this->index(index, 0);
//   const auto source_index = mapToSource(model_index);
// }

}  // namespace llm_chat