qt_add_library(MainWindow STATIC MainWindow.cpp)

target_link_libraries(MainWindow PRIVATE Qt6::Widgets Utils GameBoy Disassembler)

target_include_directories(MainWindow PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../backend)

