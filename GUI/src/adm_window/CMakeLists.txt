qt_add_library(adminlibrary STATIC
        adm_main.cpp
        adm_main_window.cpp
        adm_main_window.h
)

target_link_libraries(adminlibrary PRIVATE Qt6::Core Qt6::Widgets)
target_include_directories(adminlibrary INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})