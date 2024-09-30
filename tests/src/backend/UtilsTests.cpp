#include "utils.hpp"
#include <QString>

#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators_adapters.hpp>
#include <catch2/generators/catch_generators_random.hpp>
#include <qobject.h>


TEST_CASE("JsonGetters", "[utils]") {
    QJsonObject prefInstrMapJsonObj = Utils::getInstrMapJsonObjectFromFile(true);
    QJsonObject unprefInstrMapJsonObj = Utils::getInstrMapJsonObjectFromFile(false);

    std::vector<u8> opcodesu8 = { 
        0,
        1,
        106,
        163,
        238,
        149,
        235,
        196,
        75,
        166,
        199,
        225,
        227,
        139,
        213,
        160,
        135 
    }; // len = 17
    std::vector<std::string> opcodesStdString { 
        "0x00",
        "0x01",
        "0x6A",
        "0xA3",
        "0xEE",
        "0x95",
        "0xEB",
        "0xC4",
        "0x4B",
        "0xA6",
        "0xC7",
        "0xE1",
        "0xE3",
        "0x8B",
        "0xD5",
        "0xA0",
        "0x87"
    };

    SECTION("uint8_tToHexString") {
        for(u8 i = 0; i < opcodesu8.size(); ++i) {
            REQUIRE(Utils::uint8_tToHexString(opcodesu8[i]) == opcodesStdString[i]);
        }
    }
    
    SECTION("getInstrMapJsonObjectFromFile", "[JsonObject]") {
        REQUIRE(prefInstrMapJsonObj.size() == 256); // unprefixed and prefixed opcodes
        REQUIRE(unprefInstrMapJsonObj.size() == 256); // unprefixed and prefixed opcodes
    }

    SECTION("getBytesFromInstrJsonObject", "[JsonObject]") {
        std::vector<u8> bytes = { 1,3,1,1,2,1,1,3,1,1,1,1,1,1,1,1,1 };
        for(u8 i = 0; i < opcodesu8.size(); ++i) {
            QString opcodeQstr = QString::fromStdString(opcodesStdString[i]);
            REQUIRE(bytes[i] == Utils::getBytesFromInstrJsonObject(unprefInstrMapJsonObj.value(opcodeQstr).toObject()));
        }
    }

    SECTION("getCyclesFromInstrJsonObject", "[JsonObject]") {
        std::vector<std::pair<u8, u8>> cycles = {
            {4, 0},
            {12, 0},
            {4, 0},
            {4, 0},
            {8, 0},
            {4, 0},
            {4, 0},
            {24, 12}
        };
        for(u8 i = 0; i < cycles.size(); ++i) {
            QString opcodeQstr = QString::fromStdString(opcodesStdString[i]);
            REQUIRE(cycles[i] == Utils::getCyclesFromInstrJsonObject(unprefInstrMapJsonObj.value(opcodeQstr).toObject()));
        }
    }

    SECTION("getFlagsFromInstrJsonObject", "[JsonObject]") {
        std::vector<QString> opcodes = {"0xA3", "0xBB"};
        std::vector<Utils::flagArray> flags = {
            {Utils::Flag::setOrReset,Utils::Flag::reset,Utils::Flag::set,Utils::Flag::reset}, 
            {Utils::Flag::setOrReset,Utils::Flag::set,Utils::Flag::setOrReset,Utils::Flag::setOrReset}
        };
        for(u8 i = 0; i < flags.size(); ++i) {
            REQUIRE(flags[i] == Utils::getFlagsFromInstrJsonObject(unprefInstrMapJsonObj.value(opcodes[i]).toObject()));
        }
    }

    SECTION("setBit", "[BitManipulation]") {
        u8 byte = 0; 
        REQUIRE(Utils::setBit(byte, 0) == 0b00000001);
        REQUIRE(Utils::setBit(byte, 2) == 0b00000101);
        REQUIRE(Utils::setBit(byte, 1) == 0b00000111);
        REQUIRE(Utils::setBit(byte, 7) == 0b10000111);
    }

    SECTION("clearBit" "[BitManipulation]") {
        u8 byte = 0b10000111; 
        REQUIRE(Utils::clearBit(byte, 7) == 0b00000111);
        REQUIRE(Utils::clearBit(byte, 1) == 0b00000101);
        REQUIRE(Utils::clearBit(byte, 2) == 0b00000001);
        REQUIRE(Utils::clearBit(byte, 0) == 0b00000000);
    }

    SECTION("clearBit" "[BitManipulation]") {
        u8 byte = 0b10000111; 
        REQUIRE(Utils::clearBit(byte, 7) == 0b00000111);
        REQUIRE(Utils::clearBit(byte, 1) == 0b00000101);
        REQUIRE(Utils::clearBit(byte, 2) == 0b00000001);
        REQUIRE(Utils::clearBit(byte, 0) == 0b00000000);
    }

    SECTION("getBit" "[BitManipulation]") {
        u8 byte = 0b10000111; 
        REQUIRE(Utils::getBit(byte, 7) == 1);
        REQUIRE(Utils::getBit(byte, 6) == 0);
        REQUIRE(Utils::getBit(byte, 0) == 1);
        REQUIRE(Utils::getBit(byte, 3) == 0);
    }
}


