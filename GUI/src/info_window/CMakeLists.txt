qt_add_library(infolibrary STATIC
    info_main.cpp
    info_main_window.cpp
    info_main_window.h
)

target_link_libraries(infolibrary PRIVATE Qt6::Core Qt6::Widgets)
target_include_directories(infolibrary INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})