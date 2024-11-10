#include "PPU.hpp"
#include "InterruptController.hpp"

PPU::PPU(InterruptController& ic) 
    : ic_(ic) {
}

void PPU::reset() {

}

void PPU::setLCDStatus() {

}

bool PPU::isLCDEnabled() {
    return false;
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
