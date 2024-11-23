#include "CPU.hpp"
#include "CPUClock.hpp"
#include "Joypad.hpp"
#include "Timer.hpp"
#include "SerialDataTransfer.hpp"
#include "PPU.hpp"

#include <catch2/catch_test_macros.hpp>

struct CPUTests : CPU {
    InterruptController ic;
    CPUClock cpuClock;
    Joypad joypad { ic };
    Timer timer { ic };
    SerialDataTransfer serial { ic };
    PPU ppu { ic, cpuClock, nullptr };
    Memory memory { ic, timer, joypad, serial, ppu, cpuClock };

    CPUTests()
    :   CPU(ic, cpuClock, memory) {
    }
};

TEST_CASE_METHOD(CPUTests, "CPU") {
}

