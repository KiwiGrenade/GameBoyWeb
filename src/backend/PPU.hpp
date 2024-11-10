#pragma once

#include "utils.hpp"
#include "InterruptController.hpp"

class PPU {
public:
    PPU(InterruptController& ic);
    void update(u16 cycles);
    void reset();
    inline u8 getLY() { return LY_; };
    inline void setLY(u8 b) { LY_ = b; };

protected:
    void setLCDStatus();
    inline bool isLCDEnabled() { return Utils::getBit(LCDC_, 7); };
    void drawScanline();

    InterruptController& ic_;

    u16 scanlineCounter_;
    u8 LCDC_;   // 0xFF40
    u8 STAT_;   // 0xFF41
    u8 LY_;     // 0xFF44
    u8 LYC_;    // 0xFF45
};
