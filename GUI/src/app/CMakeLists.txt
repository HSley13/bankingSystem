qt_add_executable(MyQtProject 
        main.cpp 
        main_window.cpp 
)

target_link_libraries(MyQtProject PRIVATE 
                                Qt6::Core 
                                Qt6::Widgets 
                                adm_library 
                                adm_option_library 
                                client_library 
                                info_library
                        )



target_link_directories(MyQtProject PRIVATE 
                                /opt/homebrew/lib 
                                /usr/local/mysql-connector-c++-8.2.0/lib64
                        )


target_include_directories(MyQtProject PRIVATE 
                                /opt/homebrew/include
                                /usr/local/mysql-connector-c++-8.2.0/include/jdbc
                            )                      

set_target_properties(MyQtProject PROPERTIES MACOSX_BUNDLE ON)
