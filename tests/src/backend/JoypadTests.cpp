#include "Joypad.hpp"
#include <memory>
#include <catch2/catch_test_macros.hpp>

struct JoypadTests : Joypad {
    Memory memory_;
    CPU cpu_ = CPU(memory_);
    JoypadTests() : Joypad(memory_.joypad_, cpu_) {
    }
};

TEST_CASE_METHOD(JoypadTests, "reset") { 
    joypad_.buttons_ = 0xFF;
    reset();
    REQUIRE(joypad_.buttons_ == 0);
}

TEST_CASE_METHOD(JoypadTests, "press") {
    press(Button::A);
    REQUIRE(joypad_.buttons_ == 0b00000001);
    press(Button::B);
    REQUIRE(joypad_.buttons_ == 0b00000011);
    press(Button::Select);
    REQUIRE(joypad_.buttons_ == 0b00000111);
    press(Button::Start);
    REQUIRE(joypad_.buttons_ == 0b00001111);
    press(Button::Right);
    REQUIRE(joypad_.buttons_ == 0b00011111);
    press(Button::Left);
    REQUIRE(joypad_.buttons_ == 0b00111111);
    press(Button::Up);
    REQUIRE(joypad_.buttons_ == 0b01111111);
    press(Button::Down);
    REQUIRE(joypad_.buttons_ == 0b11111111);
}

TEST_CASE_METHOD(JoypadTests, "release") {
    joypad_.buttons_ = 0b11111111;

    release(Button::Down);
    REQUIRE(joypad_.buttons_ == 0b01111111);
    release(Button::Up);
    REQUIRE(joypad_.buttons_ == 0b00111111);
    release(Button::Left);
    REQUIRE(joypad_.buttons_ == 0b00011111);
    release(Button::Right);
    REQUIRE(joypad_.buttons_ == 0b00001111);
    release(Button::Start);
    REQUIRE(joypad_.buttons_ == 0b00000111);
    release(Button::Select);
    REQUIRE(joypad_.buttons_ == 0b00000011);
    release(Button::B);
    REQUIRE(joypad_.buttons_ == 0b00000001);
    release(Button::A);
    REQUIRE(joypad_.buttons_ == 0b00000000);
}
