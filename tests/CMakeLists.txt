cmake_minimum_required(VERSION 3.27)

project(unit-tests LANGUAGES CXX)

find_package(Qt6 REQUIRED COMPONENTS Core Test)

enable_testing(true)
add_executable(unit-tests tst_settings.cpp tst_chat_thread.cpp tst_chat_message.cpp)

cpmaddpackage(
  NAME
  Catch2
  VERSION
  3.7.1
  GITHUB_REPOSITORY
  catchorg/Catch2
  SYSTEM
  YES)

target_link_libraries(
  unit-tests
  PRIVATE libs::project_warnings
          libs::project_options
          libs::core
          libs::chat
          Catch2::Catch2WithMain)

target_link_system_libraries(
  unit-tests
  PRIVATE
  Qt::Core
  Qt::Test)

list(APPEND CMAKE_MODULE_PATH ${Catch2_SOURCE_DIR}/extras)
include(CTest)
include(Catch)
catch_discover_tests(unit-tests)
