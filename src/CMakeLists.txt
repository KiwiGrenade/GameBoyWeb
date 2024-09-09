add_executable(GameBoy main.cpp)
target_link_libraries(GameBoy PRIVATE nlohmann_json::nlohmann_json)
add_library(memory STATIC Memory.cpp)
