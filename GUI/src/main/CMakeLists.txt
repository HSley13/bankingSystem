add_executable(MyQtProject 
                        main.cpp 
                        main_window.cpp 
                )

target_link_libraries(MyQtProject PRIVATE
                                SSL
                                Crypto
                                adm_library 
                                adm_option_library 
                                client_library
                                database_library
                        )

set_target_properties(MyQtProject PROPERTIES MACOSX_BUNDLE ON)
