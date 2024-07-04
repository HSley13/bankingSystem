add_executable(MyQtProject 
                        main.cpp 
                        main_window.cpp 
                )

target_link_libraries(MyQtProject PRIVATE
                                SSL
                                Crypto
                                login_library
                                client_library
                                database_library
                        )

set_target_properties(MyQtProject PROPERTIES MACOSX_BUNDLE ON)
