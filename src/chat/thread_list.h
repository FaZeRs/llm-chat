#pragma once

#include <QAbstractListModel>

namespace llm_chat {

class Thread;

/// @brief The ThreadList class provides a model for the chat threads.
class ThreadList : public QAbstractListModel {
  Q_OBJECT
  Q_PROPERTY(QList<Thread *> threads READ threads CONSTANT FINAL)

 public:
  /// @brief The data roles for the model.
  enum ThreadRoles { TextRole = Qt::UserRole + 1, CreatedAtRole };

  /// @brief Constructs a new ThreadList object.
  /// @param parent The parent object.
  explicit ThreadList(QObject *parent = nullptr);

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

  /// @brief Returns the list of threads.
  /// @return The list of threads.
  [[nodiscard]] inline const QList<Thread *> &threads() const {
    return m_Threads;
  }
  /// @brief Creates a new thread.
  /// @details This function creates a new thread and sets it as the active
  /// thread.
  Thread *createNewThread();
  /// @brief Removes the thread at the given index.
  /// @param index The index of the thread to remove.
  void deleteThread(const QModelIndex &model_index);
  /// @brief Removes all the threads.
  void deleteAllThreads();

 private:
  QList<Thread *> m_Threads;
};

}  // namespace llm_chat