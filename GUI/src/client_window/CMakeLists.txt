qt_add_library(clientlibrary STATIC
    client_main.cpp
    client_main_window.cpp
    client_main_window.h 
)

target_link_libraries(clientlibrary PRIVATE Qt6::Core Qt6::Widgets)
target_include_directories(clientlibrary INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})