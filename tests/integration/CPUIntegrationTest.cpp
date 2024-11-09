#include "GameBoy.hpp"

#include <catch2/catch_test_macros.hpp>
#include <memory>

TEST_CASE("01-special.gb", "[tag1], [tag2]") {
    std::string testRomPath = Utils::romsPath + "/tests/Blaarg/cpu_instrs/individual/01-special.gb";
    auto car  = std::make_shared<Cartridge>(testRomPath);
    auto gb = std::make_shared<GameBoy>();

    gb->loadCartridge(car);

    for(uint32_t i = 0; i < 15000000; ++i) {
        gb->emulateStep();
    }

    std::string testOutput = gb->getSerialOutput();
    DYNAMIC_SECTION("Serial output : " << testOutput) {
        REQUIRE(testOutput.contains("Passed"));
    }
}
