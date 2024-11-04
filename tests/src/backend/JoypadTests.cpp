#include "Joypad.hpp"
#include <catch2/catch_test_macros.hpp>

struct JoypadTests : Joypad {
    InterruptController ic;
    JoypadTests() : Joypad(ic) {
    }
};

TEST_CASE_METHOD(JoypadTests, "reset") { 
    buttons_ = 0xFF;
    reset();
    REQUIRE(buttons_ == 0);
}

TEST_CASE_METHOD(JoypadTests, "press") {
    press(Button::A);
    REQUIRE(buttons_ == 0b00000001);
    press(Button::B);
    REQUIRE(buttons_ == 0b00000011);
    press(Button::Select);
    REQUIRE(buttons_ == 0b00000111);
    press(Button::Start);
    REQUIRE(buttons_ == 0b00001111);
    press(Button::Right);
    REQUIRE(buttons_ == 0b00011111);
    press(Button::Left);
    REQUIRE(buttons_ == 0b00111111);
    press(Button::Up);
    REQUIRE(buttons_ == 0b01111111);
    press(Button::Down);
    REQUIRE(buttons_ == 0b11111111);
}

TEST_CASE_METHOD(JoypadTests, "release") {
    buttons_ = 0b11111111;

    release(Button::Down);
    REQUIRE(buttons_ == 0b01111111);
    release(Button::Up);
    REQUIRE(buttons_ == 0b00111111);
    release(Button::Left);
    REQUIRE(buttons_ == 0b00011111);
    release(Button::Right);
    REQUIRE(buttons_ == 0b00001111);
    release(Button::Start);
    REQUIRE(buttons_ == 0b00000111);
    release(Button::Select);
    REQUIRE(buttons_ == 0b00000011);
    release(Button::B);
    REQUIRE(buttons_ == 0b00000001);
    release(Button::A);
    REQUIRE(buttons_ == 0b00000000);
}

TEST_CASE_METHOD(JoypadTests, "write") {
    SECTION("BUTTONS") {
        write(0b00010000);
        REQUIRE(inputDevice_ == Joypad::InputDevice::Buttons);
    }
    SECTION("PAD") {
        write(0b00100000);
        REQUIRE(inputDevice_ == Joypad::InputDevice::Pad);
    }
    SECTION("NOTHING") {
        REQUIRE(inputDevice_ == Joypad::InputDevice::Nothing);
        write(0b00110000);
        REQUIRE(inputDevice_ == Joypad::InputDevice::Nothing);
    }
}

TEST_CASE_METHOD(JoypadTests, "read") {
    buttons_ = 0b01110101;
    SECTION("BUTTONS") {
        write(0b00010000);
        REQUIRE(read() == 0b11010101);
    }
    SECTION("PAD") {
        write(0b00100000);
        REQUIRE(read() == 0b11100111);
    }
    SECTION("NOTHING") {
        write(0b00110000);
        REQUIRE(read() == 0b11111111);
    }
}
