cmake_minimum_required(VERSION 3.28.1)
project(BankingSystem)

set(CMAKE_CXX_STANDARD 17)

find_package(Qt6 COMPONENTS Core Widgets REQUIRED)
qt_standard_project_setup()

add_executable(BankingSystem main.cpp)
target_link_libraries(BankingSystem database_library Qt6::Core Qt6::Widgets)

target_link_directories(BankingSystem PRIVATE /opt/homebrew/lib)
target_link_directories(BankingSystem PRIVATE /usr/local/mysql-connector-c++-8.2.0/lib64)

target_include_directories(BankingSystem PRIVATE /opt/homebrew/include)
target_include_directories(BankingSystem PRIVATE /usr/local/mysql-connector-c++-8.2.0/include/jdbc)

add_subdirectory(GUI/src/app)
add_subdirectory(GUI/src/database)
add_subdirectory(GUI/src/adm_window)
add_subdirectory(GUI/src/adm_window/adm_option)
add_subdirectory(GUI/src/client_window)
add_subdirectory(GUI/src/info_window)
