add_executable(RegisterPairTests RegisterPairTests.cpp)

target_include_directories(RegisterPairTests PRIVATE ${CMAKE_SOURCE_DIR}/src/backend)

target_link_libraries(RegisterPairTests PRIVATE
    RegisterPair
    Catch2::Catch2WithMain
)

add_test(RegisterPairTests ${CMAKE_SOURCE_DIR}/bin/RegisterPairTests)
