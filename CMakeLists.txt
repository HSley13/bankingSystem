cmake_minimum_required(VERSION 3.28.1)
project(BankingSystem)

set(CMAKE_CXX_STANDARD 17)

find_package(Qt6 COMPONENTS Core Widgets REQUIRED)
qt_standard_project_setup()

add_executable(BankingSystem main.cpp)

target_link_libraries(BankingSystem PRIVATE database_library Qt6::Core Qt6::Widgets)

add_subdirectory(GUI/src/database)

install(TARGETS BankingSystem DESTINATION /usr/local/bin)