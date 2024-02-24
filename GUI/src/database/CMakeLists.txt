qt_add_library(database_library STATIC 
                    database.cpp
                )

target_link_libraries(database_library PUBLIC
                            Qt6::Core 
                            Qt6::Widgets
                            argon2 
                            ssl 
                            crypto 
                            mysqlcppconn
                        )

target_link_directories(database_library PUBLIC
                                /opt/homebrew/lib 
                                /usr/local/mysql-connector-c++-8.2.0/lib64
                        )


target_include_directories(database_library PUBLIC 
                                /opt/homebrew/include
                                /usr/local/mysql-connector-c++-8.2.0/include/jdbc
                            )

target_include_directories(database_library INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
