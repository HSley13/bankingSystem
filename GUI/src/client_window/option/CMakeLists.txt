qt_add_library(option_library STATIC
    option_main.cpp
    option_main_window.cpp
    option_main_window.h
)

target_link_libraries(option_library PRIVATE
                            Qt6::Core
                            Qt6::Widgets
                            database_library
                            edit_forget_library
                    )

target_include_directories(option_library INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

