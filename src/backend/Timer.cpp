#include "Timer.hpp"

#include "Processor.hpp"

Timer::Timer(Processor& cpu)
    : cpu_(cpu) {
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
        int freq = frequencies[TAC_ & 0b11];
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
    cpu_.request_interrupt(Processor::Interrupt::TIMER);
}

u8 Timer::read(u16 addr) const {
    u8 b = 0xFF;
    switch (addr) {
        case 0xFF04: b = (DIV_ >> 8); break;
        case 0xFF05: b = TIMA_; break;
        case 0xFF06: b = TMA_; break;
        case 0xFF07: b = TAC_; break;
    }
    return b;
}

void Timer::write(u8 byte, u16 addr) {
    switch (addr) {
        case 0xFF04: DIV_ = 0; break;
        case 0xFF05: TIMA_ = byte; break;
        case 0xFF06: TMA_ = byte; break;
        case 0xFF07: TAC_ = byte; break;
    }
}

void Timer::reset() {
    div_ticks_ = 0;
    tima_ticks_ = 0;
    DIV_ = 0;
    TIMA_ = 0;
    TMA_ = 0;
    TAC_ = 0;
}

