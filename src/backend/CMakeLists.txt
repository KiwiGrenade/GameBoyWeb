qt_add_executable(GameBoy main.cpp)

qt_add_library(Memory STATIC Memory.cpp)

target_link_libraries(GameBoy PRIVATE Qt6::Widgets MainWindow)

target_include_directories(GameBoy PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../frontend)

target_compile_options(GameBoy PRIVATE -pthread)
