#include "Cartridge.hpp"

#include <catch2/catch_test_macros.hpp>

constexpr u16 Cartridge::romSize_;

TEST_CASE("CartridgeTests") {
    SECTION("defaultConstructor") {
        Cartridge car;
        REQUIRE(car.getTitle() == "");
        REQUIRE(car.read(20) == 0);
    }
    std::string testRomPath = Utils::romsPath + "/games/drmario.gb";
    Cartridge car {testRomPath};
    SECTION("arrayConstructor") {
        REQUIRE(car.read(0) == 0xC3);
        REQUIRE(car.read(0x7FFE) == 0x5A);
    }
    SECTION("getTitle") {
        std::string expTitle = "DR.MARIO"; 
        REQUIRE(car.getTitle().contains(expTitle));
    }
}
