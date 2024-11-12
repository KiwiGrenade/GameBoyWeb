#include "GameBoy.hpp"

#include <catch2/catch_test_macros.hpp>
#include <memory>
#include "IntegrationTestUtils.hpp"

using namespace TestUtils::TestNames::CpuInstrs;

TEST_CASE("individual") {
    for(auto testName : individual) {
        DYNAMIC_SECTION(testName) {
            std::string testRomPath = Utils::romsPath + "/tests/Blaarg/cpu_instrs/individual/" + testName + ".gb";
            auto car  = std::make_shared<Cartridge>(testRomPath);
            auto gb = std::make_shared<GameBoy>();

            gb->loadCartridge(car);

            for(uint32_t i = 0; i < 8000000; ++i) {
                gb->emulateStep();
            }

            std::string testOutput = gb->getSerialOutput();
            DYNAMIC_SECTION("Serial output : " << testOutput) {
                CHECK(testOutput.contains("Passed"));
            }
        }
    }
}

TEST_CASE("cpu_instrs", "[tag1], [tag2]") {
    SKIP("Requires MBC1 to work!");
    std::string testRomPath = Utils::romsPath + "/tests/Blaarg/cpu_instrs/" + main + ".gb";
    auto car  = std::make_shared<Cartridge>(testRomPath);
    auto gb = std::make_shared<GameBoy>();

    gb->loadCartridge(car);

    for(uint32_t i = 0; i < 480000000; ++i) {
        gb->emulateStep();
    }

    std::string testOutput = gb->getSerialOutput();
    DYNAMIC_SECTION("Serial output : " << testOutput) {
        CHECK(testOutput.contains("Passed"));
    }
}
