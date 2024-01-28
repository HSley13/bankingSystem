qt_add_executable(MyQtProject 
        main.cpp 
        main_window.cpp 
)

target_link_libraries(MyQtProject PRIVATE Qt6::Core Qt6::Widgets databaselib adminlibrary clientlibrary infolibrary)

set_target_properties(MyQtProject PROPERTIES MACOSX_BUNDLE ON)
