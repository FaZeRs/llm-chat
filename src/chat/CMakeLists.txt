qt_add_library(chat
    STATIC
    backend.cpp
    message.cpp
    thread.cpp
    thread_list.cpp
    thread_proxy_list.cpp
)

target_include_directories(chat
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(chat PRIVATE
    libs::project_options
    libs::project_warnings
)

target_link_system_libraries(chat PRIVATE
    Qt::Core Qt::Network)

add_library(libs::chat ALIAS chat)