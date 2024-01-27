cmake_minimum_required(VERSION 3.28.1)
project(MyQtProject)

set(CMAKE_CXX_STANDARD 17)

find_package(Qt6 COMPONENTS Core Widgets REQUIRED)
qt_standard_project_setup()

add_subdirectory(src/app)
add_subdirectory(src/adm_window)
add_subdirectory(src/client_window)
add_subdirectory(src/info_window)