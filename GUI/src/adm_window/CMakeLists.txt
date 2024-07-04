add_library(adm_library STATIC 
                                adm_main.cpp
                                adm_main_window.cpp
            )

target_link_libraries(adm_library PRIVATE database_library)

target_include_directories(adm_library INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})