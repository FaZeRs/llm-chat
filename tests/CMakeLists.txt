cmake_minimum_required(VERSION 3.27)

project(unit-tests LANGUAGES CXX)

find_package(
  Qt6 REQUIRED
  COMPONENTS Core
             Gui
             Qml
             Test)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

enable_testing(true)

qt_add_executable(settings-tests tst_settings.cpp)
add_test(NAME settings-tests COMMAND settings-tests)
target_link_libraries(settings-tests PRIVATE libs::project_warnings libs::project_options libs::core)
target_link_system_libraries(
  settings-tests
  PRIVATE
  Qt::Core
  Qt::Gui
  Qt::Qml
  Qt::Test)

qt_add_executable(chat-thread-tests tst_chat_thread.cpp)
add_test(NAME chat-thread-tests COMMAND chat-thread-tests)
target_link_libraries(chat-thread-tests PRIVATE libs::project_warnings libs::project_options libs::chat)
target_link_system_libraries(
  chat-thread-tests
  PRIVATE
  Qt::Core
  Qt::Test)

qt_add_executable(chat-message-tests tst_chat_message.cpp)
add_test(NAME chat-message-tests COMMAND chat-message-tests)
target_link_libraries(chat-message-tests PRIVATE libs::project_warnings libs::project_options libs::chat)
target_link_system_libraries(
  chat-message-tests
  PRIVATE
  Qt::Core
  Qt::Test)
