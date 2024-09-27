#include "CPU.hpp"

#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators_adapters.hpp>
#include <catch2/generators/catch_generators_random.hpp>

TEST_CASE("CPU") {

    SECTION("constructor") {
        Memory memory;
        CPU cpu(memory);
        cpu.step();
        REQUIRE(true);
    }
}
