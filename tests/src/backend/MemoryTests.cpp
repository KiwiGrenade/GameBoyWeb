#include "Memory.hpp"

#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators_adapters.hpp>
#include <catch2/generators/catch_generators_random.hpp>

TEST_CASE("Memory") {
    Memory memory;

    SECTION("write / read") {
        u8 byte = GENERATE(take(100, random(0, 0xFF)));
        u16 i = GENERATE(take(100, random(0, 0xFFFF)));
        memory.write(byte, i);
        REQUIRE(memory.read(i) == byte);
    }
};
