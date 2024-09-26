add_executable(RegisterPairTests RegisterPairTests.cpp)
add_executable(UtilsTests UtilsTests.cpp)

target_include_directories(RegisterPairTests PRIVATE ${CMAKE_SOURCE_DIR}/src/backend)
target_include_directories(UtilsTests PRIVATE ${CMAKE_SOURCE_DIR}/src/backend)

target_link_libraries(RegisterPairTests PRIVATE
    RegisterPair
    Catch2::Catch2WithMain
)

target_link_libraries(UtilsTests PUBLIC
    Utils
    Qt6::Core
    Catch2::Catch2WithMain
)

add_test(RegisterPairTests ${CMAKE_SOURCE_DIR}/bin/RegisterPairTests)
add_test(UtilsTests ${CMAKE_SOURCE_DIR}/bin/UtilsTests)
