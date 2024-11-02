configure_file("config.h.in" "${CMAKE_BINARY_DIR}/configured_files/include/config.h" ESCAPE_QUOTES)

find_package(
  Qt6 REQUIRED
  COMPONENTS Core
             Gui
             Qml
             Quick)
qt_policy(SET QTP0004 NEW)
qt_standard_project_setup()

add_subdirectory(chat)
add_subdirectory(quick)
add_subdirectory(core)

set(QML_FILES
    qml/main.qml
    qml/MenuBar.qml
    qml/AboutDialog.qml
    qml/OptionsDialog.qml
    qml/SettingsTab.qml
    qml/ChatSettingsTab.qml
    qml/ShortcutsTab.qml
    qml/ShortcutRow.qml
    qml/StatusBar.qml
    qml/FpsCounter.qml
    qml/ToolBar.qml
    qml/ToolButton.qml
    qml/ChatView.qml)

set(RESOURCE_FILES assets/fonts/font-awesome-regular.otf assets/fonts/font-awesome-solid.otf
                   assets/fonts/font-awesome-brands.otf)

qt_add_executable(${CMAKE_PROJECT_NAME} main.cpp)

qt_add_qml_module(
  ${CMAKE_PROJECT_NAME}
  URI
  llm_chat
  VERSION
  1.0
  QML_FILES
  ${QML_FILES}
  RESOURCES
  qtquickcontrols2.conf
  NO_RESOURCE_TARGET_PATH)

qt_add_resources(
  ${CMAKE_PROJECT_NAME}
  "assets"
  PREFIX
  "/"
  FILES
  ${RESOURCE_FILES})

set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES MACOSX_BUNDLE TRUE WIN32_EXECUTABLE TRUE)

target_link_libraries(
  ${CMAKE_PROJECT_NAME}
  PRIVATE libs::project_options
          libs::project_warnings
          libs::chat
          libs::core
          quick_widgetsplugin)

target_link_system_libraries(
  ${CMAKE_PROJECT_NAME}
  PRIVATE
  Qt::Core
  Qt::Gui)

include(static-analyzers)

if(ENABLE_CPPCHECK)
  enable_cppcheck(${CMAKE_PROJECT_NAME} ${WARNINGS_AS_ERRORS} "")
endif()

if(ENABLE_CLANG_TIDY)
  enable_clang_tidy(${CMAKE_PROJECT_NAME} ${WARNINGS_AS_ERRORS} "")
endif()

if(ENABLE_INCLUDE_WHAT_YOU_USE)
  enable_include_what_you_use(${CMAKE_PROJECT_NAME})
endif()

# Install the executable into "${CMAKE_INSTALL_PREFIX}/bin".
install(
  TARGETS ${CMAKE_PROJECT_NAME}
  BUNDLE DESTINATION .
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

# Generate the deployment script for the target LLM-Chat.
qt_generate_deploy_qml_app_script(
  TARGET
  ${CMAKE_PROJECT_NAME}
  OUTPUT_SCRIPT
  deploy_script
  MACOS_BUNDLE_POST_BUILD
  NO_UNSUPPORTED_PLATFORM_ERROR
  DEPLOY_USER_QML_MODULES_ON_UNSUPPORTED_PLATFORM
  NO_TRANSLATIONS)

# Call the deployment script on "cmake --install".
install(SCRIPT ${deploy_script})
