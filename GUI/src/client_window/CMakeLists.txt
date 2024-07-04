add_library(client_library STATIC
                            client_main.cpp
                            client_main_window.cpp
            )

target_link_libraries(client_library PRIVATE
                                    database_library
                                    login_library
                    )

target_include_directories(client_library INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

