qt_add_library(Memory STATIC Memory.cpp)
qt_add_library(Utils STATIC utils.hpp)
qt_add_library(GameBoy STATIC GameBoy.cpp)
qt_add_library(Disassembler STATIC Disassembler.cpp)

target_link_libraries(GameBoy PRIVATE Memory)
target_link_libraries(Memory PUBLIC Qt6::Core)
target_link_libraries(Disassembler PRIVATE Qt6::Core)
