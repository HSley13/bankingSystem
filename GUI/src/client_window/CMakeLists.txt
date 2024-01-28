qt_add_library(client_library STATIC
    client_main.cpp
    client_main_window.cpp
    client_main_window.h
)

target_link_libraries(client_library PRIVATE 
                            Qt6::Core
                            Qt6::Widgets
                            database_library
                    )

target_include_directories(client_library INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
