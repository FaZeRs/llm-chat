qt_add_library(
  core
  STATIC
  application.cpp
  settings.cpp)

target_include_directories(core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(core PRIVATE libs::project_options libs::project_warnings libs::chat)

target_link_system_libraries(
  core
  PRIVATE
  Qt::Core
  Qt::Gui
  Qt::Network
  Qt::Qml)

target_include_directories(core PRIVATE "${CMAKE_BINARY_DIR}/configured_files/include")

if(USE_SENTRY)
  target_compile_definitions(core PRIVATE USE_SENTRY)
  target_link_system_libraries(core PRIVATE sentry::sentry)
endif()

add_library(libs::core ALIAS core)
