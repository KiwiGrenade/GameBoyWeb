#include "Timer.hpp"

Timer::Timer(InterruptController& ic)
    : ic_(ic) {
    reset();
}

void Timer::update(uint64_t cycles) {
    div_ticks_ += cycles;
    // prev value - 0xFF
    if(div_ticks_ >= 0xFF) {
        div_ticks_ -= 0xFF;
        ++DIV_;
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
    ic_.requestInterrupt(InterruptController::Type::Timer);
}

void Timer::reset() {
    div_ticks_ = 0;
    tima_ticks_ = 0;
    DIV_ = 0;
    TIMA_ = 0;
    TMA_ = 0;
    TAC_ = 0;
}

