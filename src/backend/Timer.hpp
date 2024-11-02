#pragma once

#include "utils.hpp"
#include "CPU.hpp"
#include "Memory.hpp"

class Timer {
public:
    Timer(Memory& mmu, CPU& cpu);

    void update(uint64_t cycles);
    void reset();

protected:
    void timaOverflow();

    u8 getDIV() { return mmu_.read(0xFF04); };
    void setDIV(u8 val) { mmu_.setDIV(val); };
    u8 getTIMA() { return mmu_.read(0xFF05); };
    void setTIMA(u8 val) { mmu_.write(val, 0xFF05); };
    u8 getTMA() { return mmu_.read(0xFF06); };
    void setTMA(u8 val) { mmu_.write(val, 0xFF06); };
    u8 getTAC() { return mmu_.read(0xFF07); };
    void setTAC(u8 val) { mmu_.write(val, 0xFF07); };

    CPU& cpu_;
    Memory& mmu_;

    int div_ticks_;
    int tima_ticks_;

    static constexpr u16 frequencies[4] { 256, 4, 16, 64 };
};
