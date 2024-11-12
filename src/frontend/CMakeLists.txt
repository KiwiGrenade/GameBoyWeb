qt_add_library(MainWindow STATIC MainWindow.cpp)
qt_add_library(QtRenderer STATIC QtRenderer.cpp)

# target_link_libraries(MainWindow PRIVATE Qt6::Widgets Utils GameBoy Disassembler)

target_include_directories(QtRenderer PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../backend)
target_include_directories(MainWindow PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../backend)

target_link_libraries(QtRenderer PUBLIC Qt6::Widgets Renderer Utils GraphicTypes)
target_link_libraries(MainWindow PRIVATE Qt6::Widgets Utils GameBoy Cartridge QtRenderer)
