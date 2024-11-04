#include "Timer.hpp"

#include <catch2/catch_test_macros.hpp>
#include <memory>

struct TimerTest : Timer {
    InterruptController ic;
    TimerTest() : Timer(ic) {};
};

TEST_CASE("constructor") {
    InterruptController ic;
    std::unique_ptr<Timer> timer = std::make_unique<Timer>(ic);
    REQUIRE(timer);
}

TEST_CASE_METHOD(TimerTest, "reset") {
    DIV_ = 1;
    TIMA_ = 1;
    TMA_ = 1;
    TAC_ = 1;

    reset();

    REQUIRE(DIV_ == 0);
    REQUIRE(TIMA_ == 0);
    REQUIRE(TMA_ == 0);
    REQUIRE(TAC_ == 0);
}

TEST_CASE_METHOD(TimerTest, "update") {
    SECTION("divider") {
        SECTION("shouldIncrementDivider") {
            update(0x00FF);
            REQUIRE(DIV_ == 1);
            update(0x00FF);
            REQUIRE(DIV_ == 2);
        }
    }
    SECTION("timerControl") {
        SECTION("shouldIncrementTIMA") {
            TAC_ = 0b00000100;
            update(0x5000);
            REQUIRE_FALSE(TIMA_ == 0);
        }
        SECTION("shouldNOTIncrementTIMA") {
            TAC_ = 0;
            update(0x5000);
            REQUIRE(TIMA_ == 0);
        }
        SECTION("shouldSetFrequencyCorrectly") {
            TAC_ = 0b00000100;
            update(256);
            REQUIRE(TIMA_ == 1);

            TAC_ = 0b00000101;
            update(40);
            REQUIRE(TIMA_ == 11);

            TAC_ = 0b00000110;
            update(32);
            REQUIRE(TIMA_ == 13);

            TAC_ = 0b00000111;
            update(128);
            REQUIRE(TIMA_ == 15);
        }
        SECTION("shouldOverflowAndRequestInterrupt") {
            TMA_ = 0xB9;
            TAC_ = 0b00000101;
            update(1024);
            REQUIRE(TIMA_ == TMA_);
        }
    }
}



