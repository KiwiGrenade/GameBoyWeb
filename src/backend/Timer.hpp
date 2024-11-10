#pragma once

#include "utils.hpp"

#include "InterruptController.hpp"

class Timer {
public:
    Timer(InterruptController& ic);

    void update(uint64_t cycles);
    void reset();
    inline u8 getDIV() { return DIV_; };
    inline void setDIV(u8 val) { DIV_ = val; };
    inline u8 getTIMA() { return TIMA_; };
    inline void setTIMA(u8 val) { TIMA_ = val; };
    inline u8 getTMA() { return TMA_; };
    inline void setTMA(u8 val) { TMA_ = val; };
    inline u8 getTAC() { return TAC_; };
    inline void setTAC(u8 val) { TAC_ = val; };

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
