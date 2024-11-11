#pragma once

#include "utils.hpp"
#include "InterruptController.hpp"
#include "GraphicTypes.hpp"

class PPU {
public:
    PPU(InterruptController& ic);
    void update(u16 cycles);
    void reset();
    u8 read(u16 addr);
    void write(u8 byte, u16 addr);

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
    void dmaTransfer(u8 b);

    InterruptController& ic_;
    
    bool statSignal_;
    bool isRenderer_;
    uint32_t clock_;
    u16 scanline_;
    
    // registers
    u8 LCDC_;   // 0xFF40
    u8 STAT_;   // 0xFF41
    u8 SCY_;    // 0xFF42
    u8 SCX_;    // 0xFF43
    u8 LY_;     // 0xFF44
    u8 LYC_;    // 0xFF45
    u8 BGP_;    // 0xFF47
    u8 OBP0_;   // 0xFF48
    u8 OBP1_;   // 0xFF49
    u8 WY_;     // 0xFF4A
    u8 WX_;     // 0xFF4B
};
