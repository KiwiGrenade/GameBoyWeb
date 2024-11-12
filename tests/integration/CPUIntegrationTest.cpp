#include "GameBoy.hpp"

#include <catch2/catch_test_macros.hpp>

std::array<std::string, 11> individual {
    "01-special",
    "02-interrupts",
    "03-op sp,hl",
    "04-op r,imm",
    "05-op rp",
    "06-ld r,r",
    "07-jr,jp,call,ret,rst",
    "08-misc instrs",
    "09-op r,r",
    "10-bit ops",
    "11-op a,(hl)"
};
TEST_CASE("individual") {
    for(auto testName : individual) {
        /*DYNAMIC_SECTION(testName) {*/
            std::cout << "Test name:" << testName << std::endl;
            std::string testRomPath = Utils::romsPath + "/tests/Blaarg/cpu_instrs/individual/" + testName + ".gb";
            auto car  = std::make_shared<Cartridge>(testRomPath);
            auto gb = std::make_shared<GameBoy>();

            gb->loadCartridge(car);

            for(uint32_t i = 0; i < 8000000; ++i) {
                gb->emulateStep();
            }

            std::string testOutput = gb->getSerialOutput();
            std::cout << "Test output: " << testOutput << std::endl;
            /*DYNAMIC_SECTION("Serial output : " << testOutput) {*/
            /*    CHECK(testOutput.contains("Passed"));*/
            /*}*/
        /*}*/
    }
}

/*TEST_CASE("cpu_instrs", "[tag1], [tag2]") {*/
/*    SKIP("Requires MBC1 to work!");*/
/*    std::string testRomPath = Utils::romsPath + "/tests/Blaarg/cpu_instrs/" + main + ".gb";*/
/*    auto car  = std::make_shared<Cartridge>(testRomPath);*/
/*    auto gb = std::make_shared<GameBoy>();*/
/**/
/*    gb->loadCartridge(car);*/
/**/
/*    for(uint32_t i = 0; i < 480000000; ++i) {*/
/*        gb->emulateStep();*/
/*    }*/
/**/
/*    std::string testOutput = gb->getSerialOutput();*/
/*    DYNAMIC_SECTION("Serial output : " << testOutput) {*/
/*        CHECK(testOutput.contains("Passed"));*/
/*    }*/
/*}*/
