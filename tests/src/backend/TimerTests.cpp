#include "Timer.hpp"

#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators_adapters.hpp>
#include <catch2/generators/catch_generators_random.hpp>
#include <memory>

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
    div_ticks_ = 1;
    tima_ticks_ = 1;
    mmu_.write(1, 0xFF05);

    reset();

    REQUIRE(div_ticks_ == 0);
    REQUIRE(tima_ticks_ == 0);
    REQUIRE(TIMA_ == 0);
    REQUIRE(TMA_ == 0);
    REQUIRE(TAC_ == 0);
    REQUIRE(DIV_ == 0);
}

TEST_CASE_METHOD(TimerTest, "update") {
    SECTION("dividerIncrementation") {
        update(0x5000);
        REQUIRE(DIV_ == 1);
    }
    REQUIRE(false);
}



