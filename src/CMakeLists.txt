qt_add_executable(GameBoyEmu main.cpp)

target_include_directories(GameBoyEmu PRIVATE frontend backend)

target_link_libraries(GameBoyEmu PRIVATE Qt6::Widgets MainWindow)

target_compile_options(GameBoyEmu PRIVATE -pthread)

add_subdirectory(backend)
add_subdirectory(frontend)
