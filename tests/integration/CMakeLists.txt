add_executable(CPUIntegrationTest CPUIntegrationTest.cpp)
add_executable(CPULogMaker CPULogMaker.cpp)

qt_add_library(IntegrationTestsUtils STATIC IntegrationTestUtils.hpp)

target_include_directories(CPUIntegrationTest PRIVATE ${CMAKE_SOURCE_DIR}/src/backend)
target_include_directories(CPULogMaker PRIVATE ${CMAKE_SOURCE_DIR}/src/backend)

target_link_libraries(CPUIntegrationTest PRIVATE
    IntegrationTestsUtils
    GameBoy
    Qt6::Core
    Catch2::Catch2WithMain
)
target_link_libraries(CPULogMaker PRIVATE
    IntegrationTestsUtils
    GameBoy
    Qt6::Core
    Catch2::Catch2WithMain
)

target_compile_options(CPULogMaker PRIVATE -Wall)

add_test(CPUIntegrationTest ${CMAKE_SOURCE_DIR}/bin/CPUIntegrationTest)
