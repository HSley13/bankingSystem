cmake_minimum_required(VERSION 3.29.2)
project(BankingSystem LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

find_package(Qt6 6.7.0 REQUIRED COMPONENTS Core Widgets REQUIRED)
qt_standard_project_setup()

add_executable(BankingSystem main.cpp)

target_link_libraries(BankingSystem PRIVATE database_library) 

add_subdirectory(GUI/src/database)
add_subdirectory(GUI/src/main)
add_subdirectory(GUI/src/login_window)
add_subdirectory(GUI/src/adm_window)
add_subdirectory(GUI/src/client_window)
add_subdirectory(GUI/src/client_window/option)
add_subdirectory(GUI/src/client_window/option/edit_forget)