add_executable(MyQtProject 
        main.cpp 
        main_window.cpp 
)

target_link_libraries(MyQtProject PRIVATE 
                                Qt6::Core 
                                Qt6::Widgets 
                                Qt6::Quick
                                OpenSSL::SSL
                                OpenSSL::Crypto
                                adm_library 
                                adm_option_library 
                                client_library
                        )

set_target_properties(MyQtProject PROPERTIES MACOSX_BUNDLE ON)
