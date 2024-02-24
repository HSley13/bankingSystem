qt_add_library(edit_forget_library STATIC
            edit_forget_main.cpp
            edit_forget_main_window.cpp
)

target_link_libraries(edit_forget_library PRIVATE
                                Qt6::Core 
                                Qt6::Widgets
                                database_library 
                        )


target_include_directories(edit_forget_library INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

