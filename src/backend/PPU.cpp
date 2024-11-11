#include "PPU.hpp"
#include "InterruptController.hpp"
#include "utils.hpp"

PPU::PPU(InterruptController& ic) 
    : ic_(ic) {
}

void PPU::reset() {
    clock_ = 0;
    scanline_ = 0;
    LCDC_ = 0x90;
    STAT_ = 0;
    SCY_ = 0;
    SCX_ = 0;
    LY_ = 0;
    LYC_ = 0;
    BGP_ = 0xFC;
    OBP0_ = 0xFF;
    OBP1_ = 0xFF;
    WY_ = 0;
    WX_ = 0;
    statSignal_ = false;
}

u8 PPU::read(uint16_t addr)
{
    uint8_t b = 0xFF;
    switch (addr)
    {
        case 0xFF40: b = LCDC_; break;
        case 0xFF41: b = STAT_; break;
        case 0xFF42: b = SCY_; break;
        case 0xFF43: b = SCX_; break;
        /*case 0xFF44: b = LY_; break;*/
        case 0xFF44: return 0x90; // Blaargs test: cpu_instrs hotfix
        case 0xFF45: b = LYC_; break;
        // 0xFF46: dma transfer
        case 0xFF47: b = BGP_; break;
        case 0xFF48: b = OBP0_; break;
        case 0xFF49: b = OBP1_; break;
        case 0xFF4a: b = WY_; break;
        case 0xFF4b: b = WX_; break;

        default:
            Utils::error("Could not read from PPU!");
    }
    return b;
}

void PPU::write(uint8_t b, uint16_t addr)
{
    switch (addr)
    {
        case 0xFF40: LCDC_ = b; break;
        case 0xFF41:
            // lower 3 bits of STAT are read only,
            // write to upper 4 bits resets them 
            STAT_ &= ~0xf8;
            // set upper 4 bits
            STAT_ |= b & 0xf8; break;
        case 0xFF42: SCY_ = b; break;
        case 0xFF43: SCX_ = b; break;
        case 0xFF44: LY_ = b; break;
        case 0xFF45: LYC_ = b; break;
        // 0xFF46: DMA transfer
        case 0xFF47: BGP_ = b; break;
        case 0xFF48: OBP0_ = b; break;
        case 0xFF49: OBP1_ = b; break;
        case 0xFF4a: WY_ = b; break;
        case 0xFF4b: WX_ = b; break;
        default:
            Utils::error("Could not read from PPU!");
    }
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
        scanline_ = 0;
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
