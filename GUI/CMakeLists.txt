cmake_minimum_required(VERSION 3.28.1)
project(MyQtProject)

set(CMAKE_CXX_STANDARD 17)

find_package(Qt6 COMPONENTS Core Widgets REQUIRED)

add_executable(MyQtProject main.cpp mainwindow.cpp database.cpp)

set_target_properties(MyQtProject PROPERTIES AUTOMOC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

target_link_libraries(MyQtProject PRIVATE argon2 ssl crypto mysqlcppconn Qt6::Core Qt6::Widgets)

target_link_directories(MyQtProject PRIVATE /opt/homebrew/lib)
target_link_directories(MyQtProject PRIVATE /usr/local/mysql-connector-c++-8.2.0/lib64)

target_include_directories(MyQtProject PRIVATE /opt/homebrew/include)
target_include_directories(MyQtProject PRIVATE /usr/local/mysql-connector-c++-8.2.0/include/jdbc)