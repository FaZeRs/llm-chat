find_package(Qt6 REQUIRED COMPONENTS Quick)

set(QUICK_FILES
    keysequenceeditor.h
    keysequenceeditor.cpp)

qt_add_library(quick_widgets STATIC)
qt_add_qml_module(quick_widgets
    URI QuickWidgets
    VERSION 1.0
    SOURCES ${QUICK_FILES}
    OUTPUT_DIRECTORY QuickWidgets
    RESOURCE_PREFIX /
)

target_link_system_libraries(quick_widgets PRIVATE
    Qt::Core
    Qt::Quick)

if(APPLE)
    set_target_properties(quick_widgets PROPERTIES MACOSX_BUNDLE TRUE)
endif()