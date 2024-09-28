#include "CPU.hpp"

#include <catch2/catch_test_macros.hpp>

struct CPUTests : CPU {
    CPUTests()
    :   CPU(memory) {

    }
    Memory memory;
};

TEST_CASE_METHOD(CPUTests, "CPU") {
    SECTION("handleFlags") {
        Utils::flagArray flags = {Utils::Flag::set, Utils::Flag::set, Utils::Flag::set, Utils::Flag::set};
        handleFlags(flags);
        REQUIRE(F_ == u8(0b11110000));

        flags = {Utils::Flag::setOrReset, Utils::Flag::reset, Utils::Flag::nothing, Utils::Flag::reset};
        handleFlags(flags);
        REQUIRE(F_ == u8(0b10100000));
    }
}

