qt_add_executable(MyQtProject 
        main.cpp 
        main_window.cpp 
)

target_link_libraries(MyQtProject PRIVATE 
                                Qt6::Core 
                                Qt6::Widgets 
                                Qt6::Quick
                                adm_library 
                                adm_option_library 
                                client_library
                                ${OPENSSL_LIBRARIES}
                        )

target_link_directories(MyQtProject PUBLIC /usr/local/mysql-8.2.0-macos13-arm64/lib)

target_include_directories(MyQtProject PUBLIC /usr/local/mysql-8.2.0-macos13-arm64/include)

set_target_properties(MyQtProject PROPERTIES MACOSX_BUNDLE ON)
