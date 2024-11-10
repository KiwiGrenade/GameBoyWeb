#include "PPU.hpp"
#include "InterruptController.hpp"
#include "utils.hpp"

PPU::PPU(InterruptController& ic) 
    : ic_(ic) {
}

void PPU::reset() {
    LY_ = 0;
    scanlineCounter_ = 0;
}

void PPU::setLCDStatus() {
    if(not isLCDEnabled()) {
        scanlineCounter_ = 0;
        LY_ = 0;
        STAT_ &= 0b11111100;
        Utils::setBit(STAT_, 0, false);
        return;
    }

    u8 currMode = STAT_ & 3;
    u8 status = STAT_;

    u8 mode = 0;
    bool requestInterrupt = false;

    // VBlank - set mode to 1 
    if(LY_ >= 144) {
        mode = 1;
        Utils::setBit(status, 0, true); 
        Utils::setBit(status, 1, false);
        requestInterrupt = Utils::getBit(status, 4);
    }
    else {
        constexpr int mode2bounds = 80;
        constexpr int mode3bounds = mode2bounds + 172;

        // mode 2
        if(scanlineCounter_ <= mode2bounds) {
            mode = 2;
            Utils::setBit(status, 0, false);
            Utils::setBit(status, 1, true);
            requestInterrupt = Utils::getBit(status, 5);
        }
        // mode 3
        else if(scanlineCounter_ <= mode3bounds) {
            mode = 3;
            Utils::setBit(status, 0, true);
            Utils::setBit(status, 1, true);
        }
        // mode 0
        else {
            mode = 0;
            Utils::setBit(status, 0, false);
            Utils::setBit(status, 1, false);
            requestInterrupt = Utils::getBit(status, 3);
        }
    }
    
    // request an interrupt when entering new mode
    if(requestInterrupt && (currMode != mode)) {
        ic_.requestInterrupt(InterruptController::Type::LCD);
    }

    if(LY_ == LYC_) {
        Utils::setBit(status, 2, true);
        if(Utils::getBit(status, 6))
            ic_.requestInterrupt(InterruptController::Type::LCD);
    }
    else {
        Utils::setBit(status, 2, 0);
    }

    STAT_ = status;
}

void PPU::drawScanline() {

}

void PPU::update(u16 cycles) {
    setLCDStatus();

    if(not isLCDEnabled())
        return;

    scanlineCounter_ += cycles;

    if(scanlineCounter_ <= 456) {
        // move onto next scanline
        ++LY_;
        scanlineCounter_ = 0;
        
        // enter VBlank mode
        if(LY_ < 144) {
            drawScanline();
        }
        // reset scanline
        else if (LY_ == 144) {
            ic_.requestInterrupt(InterruptController::Type::VBlank);
        }
        else if (LY_ > 153) {
            LY_ = 0;
        }
    }

}
