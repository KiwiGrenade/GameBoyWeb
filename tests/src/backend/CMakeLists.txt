add_executable(RegisterPairTests RegisterPairTests.cpp)
add_executable(UtilsTests UtilsTests.cpp)
add_executable(MemoryTests MemoryTests.cpp)
add_executable(CPUTests CPUTests.cpp)
add_executable(ProceduresPrefixedTests ProceduresPrefixedTests.cpp)
add_executable(ProceduresUnprefixedTests ProceduresUnprefixedTests.cpp)

target_include_directories(RegisterPairTests PRIVATE ${CMAKE_SOURCE_DIR}/src/backend)
target_include_directories(UtilsTests PRIVATE ${CMAKE_SOURCE_DIR}/src/backend)
target_include_directories(MemoryTests PRIVATE ${CMAKE_SOURCE_DIR}/src/backend)
target_include_directories(CPUTests PRIVATE ${CMAKE_SOURCE_DIR}/src/backend)
target_include_directories(ProceduresPrefixedTests PRIVATE ${CMAKE_SOURCE_DIR}/src/backend)
target_include_directories(ProceduresUnprefixedTests PRIVATE ${CMAKE_SOURCE_DIR}/src/backend)

target_link_libraries(RegisterPairTests PRIVATE
    RegisterPair
    Catch2::Catch2WithMain
)
target_link_libraries(UtilsTests PUBLIC
    Utils
    Qt6::Core
    Catch2::Catch2WithMain
)
target_link_libraries(MemoryTests PUBLIC
    Memory
    Qt6::Core
    Catch2::Catch2WithMain
)
target_link_libraries(CPUTests PUBLIC
    CPU
    Qt6::Core
    Catch2::Catch2WithMain
)
target_link_libraries(ProceduresPrefixedTests PUBLIC
    CPU
    Qt6::Core
    Catch2::Catch2WithMain
)
target_link_libraries(ProceduresUnprefixedTests PUBLIC
    CPU
    Qt6::Core
    Catch2::Catch2WithMain
)

add_test(RegisterPairTests ${CMAKE_SOURCE_DIR}/bin/RegisterPairTests)
add_test(UtilsTests ${CMAKE_SOURCE_DIR}/bin/UtilsTests)
add_test(MemoryTests ${CMAKE_SOURCE_DIR}/bin/MemoryTests)
add_test(CPUTests ${CMAKE_SOURCE_DIR}/bin/CPUTests)
add_test(ProceduresPrefixedTests ${CMAKE_SOURCE_DIR}/bin/ProceduresPrefixedTests)
add_test(ProceduresUnprefixedTests ${CMAKE_SOURCE_DIR}/bin/ProceduresUnprefixedTests)
