#include "Timer.hpp"

Timer::Timer(Memory& mmu, CPU& cpu)
    : mmu_(mmu)
    , cpu_(cpu) {
    reset();
}

void Timer::update(uint64_t cycles) {
    div_ticks_ += cycles;
    // prev value - 0xFF
    if(div_ticks_ >= 0x4000) {
        div_ticks_ -= 0x4000;
        setDIV(getDIV() + 1);
    }

    // bit 2 - enable
    u8 TAC = getTAC();
    if(TAC & 0b100) {
        tima_ticks_ += cycles;
        // bit 0 and 1 - timer control (TAC)
        u16 freq = frequencies[TAC & 0b11];
        while(tima_ticks_ >= freq) {
            tima_ticks_ -= freq;
            setTIMA(getTIMA() + 1);
            if(getTIMA() == 0)
                timaOverflow();
        }
    }
}

void Timer::timaOverflow() {
    setTIMA(getTMA());
    cpu_.requestInterrupt();
}

void Timer::reset() {
    div_ticks_ = 0;
    tima_ticks_ = 0;
    mmu_.resetTimer();
}

