#pragma once

#include <QSortFilterProxyModel>

namespace llm_chat {

// class Thread;

/// @brief The ThreadProxyList class provides a sorted model for the chat
class ThreadProxyList : public QSortFilterProxyModel {
  Q_OBJECT

 public:
  /// @brief Constructs a new ThreadProxyList object.
  /// @param parent The parent object.
  explicit ThreadProxyList(QObject *parent = nullptr);

  // Q_INVOKABLE [[nodiscard]] Thread *get(const int index) const;

 protected:
  /// @brief Returns true if the left item is less than the right item.
  /// @param left The left model index.
  /// @param right The right model index.
  /// @return True if the left item is less than the right item.
  [[nodiscard]] bool lessThan(const QModelIndex &left,
                              const QModelIndex &right) const override;
};

}  // namespace llm_chat
