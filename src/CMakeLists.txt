qt_add_executable(GameBoy main.cpp)

qt_add_library(Memory STATIC Memory.cpp)
qt_add_library(MainWindow STATIC MainWindow.cpp)

target_link_libraries(MainWindow PRIVATE Qt6::Widgets)
target_link_libraries(GameBoy PRIVATE Qt6::Widgets MainWindow)

target_compile_options(GameBoy PRIVATE -pthread)

