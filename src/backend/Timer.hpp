#pragma once

#include "InterruptController.hpp"
#include "utils.hpp"

class Timer {
public:
    Timer(InterruptController& ic);

    void update(uint64_t cycles);
    void reset();
    u8 read(u16 addr) const;
    void write(u8 byte, u16 addr);

protected:
    void timaOverflow();

    InterruptController& ic_;

    int div_ticks_;
    int tima_ticks_;

    u8 TIMA_ = 0;
    u8 TMA_ = 0;
    u8 TAC_ = 0;
    u16 DIV_ = 0;
    static constexpr int frequencies[4] { 1024, 16, 64, 256 };
};
