#include "CPU.hpp"

#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators_adapters.hpp>
#include <catch2/generators/catch_generators_random.hpp>

TEST_CASE("PREFIXED", "[PROCEDURES]") {
    Memory memory;
    CPU cpu(memory);
    memory.write(0xCB, 0);
    cpu.step();

    SECTION("0x00") {
        memory.write(0x00, 0);
    }
}
