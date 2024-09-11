qt_add_library(Memory STATIC Memory.cpp)
qt_add_library(Utils STATIC utils.hpp)
qt_add_library(GameBoy STATIC GameBoy.cpp)

target_link_libraries(GameBoy PRIVATE Memory)
target_link_libraries(Memory PUBLIC Qt6::Core)
