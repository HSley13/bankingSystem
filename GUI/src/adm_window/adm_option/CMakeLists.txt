qt_add_library(adm_option_library STATIC 
    adm_option_main.cpp
    adm_option_main_window.cpp
    adm_option_main_window.h
)

target_link_libraries(adm_option_library PRIVATE
                            Qt6::Core 
                            Qt6::Widgets
                            database_library 
                    )

target_link_directories(adm_option_library PRIVATE 
                                /opt/homebrew/lib 
                                /usr/local/mysql-connector-c++-8.2.0/lib64
                        )

target_include_directories(adm_option_library PRIVATE 
                                /opt/homebrew/include
                                /usr/local/mysql-connector-c++-8.2.0/include/jdbc
                            )

target_include_directories(adm_option_library INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})