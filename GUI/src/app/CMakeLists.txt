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
                   

set_target_properties(MyQtProject PROPERTIES MACOSX_BUNDLE ON)
