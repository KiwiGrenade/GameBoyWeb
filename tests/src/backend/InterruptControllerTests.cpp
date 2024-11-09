#include "InterruptController.hpp"

#include <catch2/catch_test_macros.hpp>

TEST_CASE_METHOD(InterruptController, "defaultConstructor") {
    REQUIRE(IE_ == 0);
    REQUIRE(IF_ == 0);
}

TEST_CASE_METHOD(InterruptController, "setIE") {
    u8 val = 0xC1;
    setIE(val);
    REQUIRE(IE_ == val);
}

TEST_CASE_METHOD(InterruptController, "getIE") {
    u8 val = 0xC2;
    IE_ = val;

    REQUIRE(getIE() == val);
}

TEST_CASE_METHOD(InterruptController, "setIF") {
    u8 val = 0xC1;
    setIF(val);

    REQUIRE(IF_ == val);
}

TEST_CASE_METHOD(InterruptController, "getIF") {
    u8 val = 0xC2;
    IF_ = val;

    REQUIRE(getIF() == val);
}

TEST_CASE_METHOD(InterruptController, "reset") {
    IE_ = 200;
    IF_ = 200;

    reset();

    REQUIRE(IE_ == 0);
    REQUIRE(IF_ == 0);
}

TEST_CASE_METHOD(InterruptController, "requestInterrupt") {
    SECTION("VBlank") {
        requestInterrupt(0);
        REQUIRE(IF_ == 0b00000001);
    }
    SECTION("LCD") {
        requestInterrupt(1);
        REQUIRE(IF_ == 0b00000010);
    }
    SECTION("Timer") {
        requestInterrupt(2);
        REQUIRE(IF_ == 0b00000100);
    }
    SECTION("Serial") {
        requestInterrupt(3);
        REQUIRE(IF_ == 0b00001000);
    }
    SECTION("Joypad") {
        requestInterrupt(4);
        REQUIRE(IF_ == 0b00010000);
    }
}
