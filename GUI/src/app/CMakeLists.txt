qt_add_executable(MyQtProject 
        main.cpp 
        main_window.cpp 
        database.cpp
)

target_link_libraries(MyQtProject PRIVATE argon2 ssl crypto mysqlcppconn Qt6::Core Qt6::Widgets adminlibrary clientlibrary infolibrary)

target_link_directories(MyQtProject PRIVATE /opt/homebrew/lib)
target_link_directories(MyQtProject PRIVATE /usr/local/mysql-connector-c++-8.2.0/lib64)

target_include_directories(MyQtProject PRIVATE /opt/homebrew/include)
target_include_directories(MyQtProject PRIVATE /usr/local/mysql-connector-c++-8.2.0/include/jdbc)

set_target_properties(MyQtProject PROPERTIES MACOSX_BUNDLE ON)

install(TARGETS MyQtProject DESTINATION bin)