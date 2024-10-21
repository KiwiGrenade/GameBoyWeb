#include "Cartridge.hpp"
#include <QByteArray>

#include <catch2/catch_test_macros.hpp>
#include <qstringview.h>

constexpr u16 Cartridge::romSize_;

TEST_CASE("CartridgeTests") {
    SECTION("defaultConstructor") {
        Cartridge car;
        REQUIRE(car.getTitle() == "");
        REQUIRE(car.read(20) == 0);
    }
    SECTION("arrayConstructor") {
        QByteArray arr = QByteArray(0x8000, 0x23);
        CHECK(arr.size() <= 0x8000);
        Cartridge car = Cartridge(arr);
    }
    QByteArray arr = QByteArray(0x8000, 0x23);
    Cartridge car = Cartridge(arr);
    SECTION("read") {
        REQUIRE(car.read(0x23) == 0x23);
    }
    SECTION("getTitle") {
        REQUIRE(car.getTitle() == "###############");
    }
}
