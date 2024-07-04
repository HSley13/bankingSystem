add_library(login_library STATIC
                        login_main.cpp
                        login_window.cpp
                )

target_link_libraries(login_library PRIVATE
                                adm_library
                                option_library
                                database_library 
                        )

target_include_directories(login_library INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})


