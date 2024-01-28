qt_add_library(adm_library STATIC
        adm_main.cpp
        adm_main_window.cpp
        adm_main_window.h
)

target_link_libraries(adm_library PRIVATE
                                Qt6::Core 
                                Qt6::Widgets
                                adm_option_library 
                                database_library 
                        )

target_include_directories(adm_library INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})


