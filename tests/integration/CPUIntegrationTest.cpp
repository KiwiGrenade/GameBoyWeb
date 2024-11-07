#include "GameBoy.hpp"

#include <catch2/catch_test_macros.hpp>
#include <memory>

TEST_CASE("01-special.gb", "[tag1], [tag2]") {
    auto car  = std::make_shared<Cartridge>("/home/jaskow/Repo/GameBoyWeb/roms/tests/Blaarg/cpu_instrs/individual/01-special.gb");


    GameBoy gb;
    gb.loadCartridge(car);

    for(uint32_t i = 0; i < 50; ++i) {
        gb.emulateStep();
    }
    DYNAMIC_SECTION("Check if : " << gb.getSerialOutput() << " contains : Passed") {
        REQUIRE_FALSE(gb.getSerialOutput().empty());
        REQUIRE(gb.getSerialOutput().contains("Passed"));
    }
}
