#pragma once

#include "utils.hpp"
#include "CPU.hpp"

class Timer {
public:
    Timer(Memory& mmu, CPU& cpu);

    void update(uint64_t cycles);
    void reset();

private:
    void timaOverflow();

    int div_ticks_;
    int tima_ticks_;
    CPU& cpu_;
    Memory& mmu_;

    u8& TIMA_;
    u8& TMA_;
    u8& TAC_;
    u16& DIV_;

    static constexpr u16 frequencies[4] {1024, 16, 64, 256};
};
