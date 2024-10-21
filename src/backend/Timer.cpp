#include "Timer.hpp"

Timer::Timer(Memory& mmu, CPU& cpu)
    : mmu_(mmu)
    , cpu_(cpu)
    , TIMA_(mmu_.timer_.TIMA_)
    , TMA_(mmu_.timer_.TMA_)
    , TAC_(mmu_.timer_.TAC_)
    , DIV_(mmu_.timer_.DIV_) {
    reset();
}

void Timer::update(uint64_t cycles) {
    div_ticks_ += cycles;
    if(div_ticks_ >= 0xFF) {
        div_ticks_-= 0xFF;
        DIV_++;
    }

    // bit 2 - enable
    if(TAC_ & 0b100) {
        tima_ticks_ += cycles;
        // bit 0 and 1 - timer control (TAC)
        u16 freq = frequencies[TAC_ & 0b11];
        while(tima_ticks_ >= freq) {
            tima_ticks_ -= freq;
            ++TIMA_;
            if(TIMA_ == 0)
                timaOverflow();
        }
    }
}

void Timer::timaOverflow() {
    TIMA_ = TMA_;
    cpu_.requestInterrupt();
}

void Timer::reset() {
    div_ticks_ = 0;
    tima_ticks_ = 0;
    mmu_.timer_.reset();
}

