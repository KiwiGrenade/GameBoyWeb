#include "GameBoy.hpp"

#include <catch2/catch_test_macros.hpp>
#include <memory>
#include "IntegrationTestUtils.hpp"

TEST_CASE("cpu_instrs", "[tag1], [tag2]") {
    for(auto testName : TestUtils::blaargTestNames) {
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
