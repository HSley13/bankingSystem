qt_add_library(clientlibrary STATIC
    client_main.cpp
    client_main_window.cpp
    client_main_window.h
    client_database.cpp 
    client_database.h 
)

target_link_libraries(clientlibrary PRIVATE Qt6::Core Qt6::Widgets)
target_link_directories(clientlibrary PRIVATE /opt/homebrew/lib)
target_link_directories(clientlibrary PRIVATE /usr/local/mysql-connector-c++-8.2.0/lib64)

target_include_directories(clientlibrary INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(clientlibrary PRIVATE /opt/homebrew/include)
target_include_directories(clientlibrary PRIVATE /usr/local/mysql-connector-c++-8.2.0/include/jdbc)