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
    void checkStatus();
    void loadSprites();
    void renderScanline();
    void handleOamScan();
    void handleVramRead();
    void handleHBlank();
    void handleVBlank();
    void setMode(u8 model);
    inline u8 getMode() { return STAT_ & 3; };
    void drawScanline();

    InterruptController& ic_;
    
    bool statSignal_;
    bool isRenderer_;
    uint32_t clock_;
    u16 scanlineCounter_;
    u8 LCDC_;   // 0xFF40
    u8 STAT_;   // 0xFF41
    u8 LY_;     // 0xFF44
    u8 LYC_;    // 0xFF45
};
