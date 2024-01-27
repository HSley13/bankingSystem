qt_add_library(adminlibrary STATIC
        adm_main.cpp
        adm_main_window.cpp
        adm_main_window.h
        adm_database.cpp
        adm_database.h
)

target_link_libraries(adminlibrary PRIVATE Qt6::Core Qt6::Widgets)
target_link_directories(adminlibrary PRIVATE /opt/homebrew/lib)
target_link_directories(adminlibrary PRIVATE /usr/local/mysql-connector-c++-8.2.0/lib64)

target_include_directories(adminlibrary INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(adminlibrary PRIVATE /opt/homebrew/include)
target_include_directories(adminlibrary PRIVATE /usr/local/mysql-connector-c++-8.2.0/include/jdbc)

