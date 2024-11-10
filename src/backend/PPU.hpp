#pragma once

#include "utils.hpp"
#include "InterruptController.hpp"

class PPU {
public:
    PPU(InterruptController& ic);
    void update(u16 cycles);
    void reset();

protected:
    void setLCDStatus();
    bool isLCDEnabled();
    void drawScanline();

    InterruptController& ic_;

    u16 scanlineCounter_;
    u8 LY_; // 0xFF44
};
