#include "PPU.hpp"
#include "InterruptController.hpp"
#include "utils.hpp"

PPU::PPU(InterruptController& ic) 
    : ic_(ic) {
}

void PPU::reset() {
    clock_ = 0;
    scanlineCounter_ = 0;
    LCDC_ = 0x90;
    STAT_ = 0;
    LY_ = 0;
    LYC_ = 0;
    statSignal_ = false;
}

void PPU::drawScanline() {

}

void PPU::setMode(u8 mode) {
    if(mode < 4)
        STAT_ &= (0b11111100 | mode);
    else
        Utils::error("Unhandled mode value in PPU!");
}

void PPU::loadSprites() {

}

void PPU::renderScanline() {

}

// OAM_SCAN mode 2 -> 3
void PPU::handleOamScan() {
    if(clock_ < 80)
        return;
    clock_ -= 80;
    loadSprites();
    // entering VRAM_READ
    setMode(3);
}

// VRAM_READ mode 3 -> 0
void PPU::handleVramRead() {
    if(clock_ < 172)
        return;
    clock_ -= 172;
    if(not isRenderer_)
        return;
    renderScanline();
    // entering HBlank
    setMode(0);

}

// HBLANK mode 0 -> 1, 0 -> 2
void PPU::handleHBlank() {
    if(clock_ < 204)
        return;

    clock_ -= 204;
    ++LY_;
    // enter VBlank mode 1 
    if(LY_ == 144) {
        scanlineCounter_ = 0;
        setMode(1);
        ic_.requestInterrupt(InterruptController::Type::VBlank);
        if(not isRenderer_)
            return;
        // TODO: Add renderer::showScreen
    }
    // enter OAM_SCAN mode 2
    else {
        setMode(2);
    }
}

void PPU::handleVBlank() {
    if(clock_ < 456)
        return;
    clock_ -= 456;

    ++LY_;
    // enter OAM_SCAN mode 2
    if(LY_ > 153) {
        setMode(2);
        LY_ = 0;
    }
}

void PPU::checkStatus() {
    u8 mode = getMode();
    bool isLYC { LY_ == LYC_ && Utils::getBit(STAT_, 6) };
    bool isVBL { mode == 0 && Utils::getBit(STAT_, 3) };
    bool isOAM { mode == 2 && Utils::getBit(STAT_, 5) };
    bool isHBL { mode == 1 && 
        (Utils::getBit(STAT_, 4) || Utils::getBit(STAT_, 5))
    };

    Utils::setBit(STAT_, 2, LY_ == LYC_);

    if(isLYC || isVBL || isOAM || isHBL) {
        if(not statSignal_)
            ic_.requestInterrupt(InterruptController::Type::LCD);
        statSignal_ = true;
    }
    else {
        statSignal_ = false;
    }
}

void PPU::update(u16 cycles) {
    clock_ += cycles;

    if(not isLCDEnabled()) {
        clock_ = 0;
        LY_ = 0;
        setMode(0);
    }

    checkStatus();

    switch (getMode()) {
        // mode 2 - scan for OAM sprites
        case 2:
            handleOamScan();
            break;
        case 3:
            handleVramRead();
            break;
        case 0:
            handleHBlank();
            break;
        case 1:
            handleVBlank();
            break;
    }
}
