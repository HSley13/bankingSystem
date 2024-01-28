qt_add_library(databaselib STATIC 
    database.cpp
    database.h
)

target_link_directories(databaselib PRIVATE argon2 ssl crypto mysqlcppconn)

target_link_directories(databaselib PRIVATE /opt/homebrew/lib)
target_link_directories(databaselib PRIVATE /usr/local/mysql-connector-c++-8.2.0/lib64)

target_include_directories(databaselib PRIVATE /opt/homebrew/include)
target_include_directories(databaselib PRIVATE /usr/local/mysql-connector-c++-8.2.0/include/jdbc)
target_include_directories(databaselib INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
