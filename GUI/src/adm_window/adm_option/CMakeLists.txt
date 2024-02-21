qt_add_library(adm_option_library STATIC 
    adm_option_main.cpp
    adm_option_main_window.cpp
    adm_option_main_window.h
)

target_link_libraries(adm_option_library PUBLIC
                            Qt6::Core 
                            Qt6::Widgets
                            database_library 
                    )

target_include_directories(adm_option_library INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})