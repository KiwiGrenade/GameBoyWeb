#include "Timer.hpp"

#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators_adapters.hpp>
#include <catch2/generators/catch_generators_random.hpp>
#include <memory>

constexpr u16 DIV = 0xFF04;
constexpr u16 TIMA = 0xFF05;
constexpr u16 TMA = 0xFF06;
constexpr u16 TAC = 0xFF07;

struct TimerTest : Timer {
    Memory mmu;
    CPU cpu = CPU(mmu);
    TimerTest() : Timer(mmu, cpu) {};
};

TEST_CASE("constructor") {
    Memory memory;
    CPU cpu = CPU(memory);
    std::unique_ptr<Timer> timer = std::make_unique<Timer>(memory, cpu);
    REQUIRE(timer != nullptr);
}

TEST_CASE_METHOD(TimerTest, "reset") {
    mmu.write(1, DIV);
    mmu.write(1, TIMA);
    mmu.write(1, TMA);
    mmu.write(1, TAC);

    reset();

    REQUIRE(mmu.read(DIV) == 0);
    REQUIRE(mmu.read(TIMA) == 0);
    REQUIRE(mmu.read(TMA) == 0);
    REQUIRE(mmu.read(TAC) == 0);
}

TEST_CASE_METHOD(TimerTest, "update") {
    SECTION("divider") {
        SECTION("shouldIncrementDivider") {
            update(0x00FF);
            REQUIRE(mmu.read(DIV) == 1);
            update(0x00FF);
            REQUIRE(mmu.read(DIV) == 2);
        }
    }
    SECTION("timerControl") {
        SECTION("shouldIncrementTIMA") {
            mmu.write(0b00000100, TAC);
            update(0x5000);
            REQUIRE_FALSE(mmu.read(TIMA) == 0);
        }
        SECTION("shouldNOTIncrementTIMA") {
            mmu.write(0b00000000, TAC);
            update(0x5000);
            REQUIRE(mmu.read(TIMA) == 0);
        }
        SECTION("shouldSetFrequencyCorrectly") {
            mmu.write(0b00000100, TAC);
            update(256);
            REQUIRE(mmu.read(TIMA) == 1);

            mmu.write(0b00000101, TAC);
            update(40);
            REQUIRE(mmu.read(TIMA) == 11);

            mmu.write(0b00000110, TAC);
            update(32);
            REQUIRE(mmu.read(TIMA) == 13);

            mmu.write(0b00000111, TAC);
            update(128);
            REQUIRE(mmu.read(TIMA) == 15);
        }
        SECTION("shouldOverflowAndRequestInterrupt") {
            mmu.write(0xB9, TMA);
            mmu.write(0b00000101, TAC);
            update(1024);
            REQUIRE(mmu.read(TIMA) == mmu.read(TMA));
        }
    }
}



