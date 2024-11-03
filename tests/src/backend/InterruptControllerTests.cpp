#include "InterruptController.hpp"

#include <memory>

#include <catch2/catch_test_macros.hpp>

TEST_CASE("InterruptController", "Constructor") {
    std::shared_ptr<InterruptController> ic = std::make_shared<InterruptController>();
    REQUIRE(ic);
}

TEST_CASE_METHOD(InterruptController, "write") {
    u8 val = 0xC1;
    write(val);
    REQUIRE(interrupts_ == val);
}

TEST_CASE_METHOD(InterruptController, "read") {
    u8 val = 0xC2;
    write(val);
    REQUIRE(read() == val);
}

TEST_CASE_METHOD(InterruptController, "reset") {
    interrupts_ = 200;
    reset();
    REQUIRE(interrupts_ == 0);
}

TEST_CASE_METHOD(InterruptController, "requestInterrupt") {
    SECTION("VBlank") {
        requestInterrupt(Type::VBlank);
        REQUIRE(interrupts_ == 0b00000001);
    }
    SECTION("LCD") {
        requestInterrupt(Type::LCD);
        REQUIRE(interrupts_ == 0b00000010);
    }
    SECTION("Timer") {
        requestInterrupt(Type::Timer);
        REQUIRE(interrupts_ == 0b00000100);
    }
    SECTION("Serial") {
        requestInterrupt(Type::Serial);
        REQUIRE(interrupts_ == 0b00001000);
    }
    SECTION("Joypad") {
        requestInterrupt(Type::Joypad);
        REQUIRE(interrupts_ == 0b00010000);
    }
}
