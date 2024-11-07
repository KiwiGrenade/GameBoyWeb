add_executable(CPUIntegrationTest CPUIntegrationTest.cpp)

target_include_directories(CPUIntegrationTest PRIVATE ${CMAKE_SOURCE_DIR}/src/backend)

target_link_libraries(CPUIntegrationTest PRIVATE
    GameBoy
    Qt6::Core
    Catch2::Catch2WithMain
)

add_test(CPUIntegrationTest ${CMAKE_SOURCE_DIR}/bin/CPUIntegrationTest)
