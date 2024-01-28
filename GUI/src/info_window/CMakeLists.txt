qt_add_library(info_library STATIC
                    info_main.cpp
                    info_main_window.cpp
                    info_main_window.h
                )

target_link_libraries(info_library PRIVATE Qt6::Core Qt6::Widgets)

target_include_directories(info_library INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})