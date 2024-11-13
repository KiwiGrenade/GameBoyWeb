#include "PPU.hpp"

#include "CPU.hpp"
#include "Memory.hpp"
#include "Renderer.hpp"
#include "GraphicTypes.hpp"
#include "InterruptController.hpp"
#include "utils.hpp"
#include <array>
#include <cstdint>


PPU::PPU(InterruptController& ic, Memory& memory, const CPU& cpu, Renderer* r)
    : ic_(ic)
    , memory_(memory)
    , cpu_(cpu)
    , renderer_(r){
}

void PPU::reset() {
    cycles_ = 0;
    windowLine_ = 0;
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
        case 0xFF40:
            LCDC_ = b;
            break;
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
            Utils::error("Could not write to PPU!");
    }
}

void PPU::update(uint32_t cycles) {
    cycles_ += cycles;

    if(not isEnabled()) {
        /*Utils::warning("PPU not enabled!");*/
        cycles_ = 0;
        LY_ = 0;
        setMode(0);
        return;
    }

    checkStatus();

    switch (getMode()) {
        // mode 2 - scan for OAM sprites
        case 2:
            Utils::warning("oam scna!");
            handleOamScan();
            break;
        case 3:
            Utils::warning("Vram read !");
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

Texture PPU::getTile(u16 i) const {
    Texture tex(8, 8);
    u16 tileBase = 0x8000;
    for(u8 byte = 0; byte < 16; byte += 2) {
        u16 idx = byte + tileBase + i * 16;
        u8 loByte = readVram(0, idx);
        u8 hiByte = readVram(0, idx + 1);

        for(u8 pix = 0; pix < 8; ++pix) {
            bool hiBit = Utils::getBit(hiByte, 7-pix);
            bool loBit = Utils::getBit(loByte, 7-pix);

            u8 colorIdx = static_cast<u8>(hiBit << 1 | loBit);
            Color c = dmgPalette[colorIdx];
            u8 p = (byte / 2) * 8 + pix;
            tex.setPixelColor(p, c);
            tex.setPixelPriority(p, 0);
        }
    }
    return tex;
}

u8 PPU::readVram(u8 bank, u16 addr) const {
    return memory_.readVram(bank, addr);
}

void PPU::renderLayerLine(Texture& tex, Layer layer) {
    // Window won't draw if current line isn't a window line
    if(layer == Layer::Window && LY_ < WY_)
        return;
    // y of current layer
    u8 y = 0;
    if(layer == Layer::Background)
        y = LY_ + SCY_;
    else if(layer == Layer::Window)
        y = windowLine_;
    
    // draw pixels in windowLine
    u8 windowPixelsDrawn = 0;
    for(u8 xPix = 0; xPix < 160; ++xPix) {
        if(layer == Layer::Window) {
            // check if windows is later in line
            if(xPix < WX_ - 7)
                continue;
            // windows isn't in this line
            else if(WX_ < 7 || WX_ > 166 || WY_ > 143)
                return;
        }

        u8 x = (layer == Layer::Background) ? xPix + SCX_ : xPix - (WX_-7);
        renderLayerPixel(tex, layer, x, y, xPix, 0);
        ++windowPixelsDrawn;
    }
    if(layer == Layer::Window && windowPixelsDrawn != 0)
        ++windowLine_;
}

// render layer pixel at (texX, texY) with color from (x, y) in vram
void PPU::renderLayerPixel(Texture& tex, Layer layer, u8 x, u8 y, u8 texX, u8 texY) const {
    u16 tileMap = 0x9800;
    if(layer == Layer::Background) {
        tileMap = Utils::getBit(LCDC_, 3) ? 0x9C00 : 0x9800;
    }
    else if(layer == Layer::Window) {
        tileMap = Utils::getBit(LCDC_, 6) ? 0x9C00 : 0x9800;
    }

    // tile data offset in VRAM
    u16 tileBase = Utils::getBit(LCDC_, 4) ? 0x8000 : 0x9000;
    u8 tileX = x >> 3;
    u8 tileY = y >> 3;
    u16 tileIdx = (tileY*32) + tileX + tileMap;
    u8 tileAttr = 0;
    u8 tileDataI = readVram(0, tileIdx);
    u8 bank = Utils::getBit(tileAttr, 3) ? 1 : 0;
    u16 adr = 0;

    // 0x9000 -> signed addressing
    if(tileBase == 0x9000)
        adr = tileBase + static_cast<int8_t>(tileDataI) * 16;
    // 0x8000 -> unsigned addressing
    else
        adr = tileBase + tileDataI * 16;
    
    // point addr to the right byte in tile data, based on y position
    // take into account vectical flip
    if(Utils::getBit(tileAttr, 6))
        adr += (7 - (y % 8)) * 2;
    else
        adr += (y % 8) * 2;
    
    // get one row of pixels (2 consecutive bytes)
    u8 loByte = readVram(0, adr);
    u8 hiByte = readVram(0, adr+1);
    // get pixel offset of tile (take horizontal flip into account)
    u8 pxOffset = 
        Utils::getBit(tileAttr, 5)
        ? 7 - (x % 8)
        : (x % 8);
    
    // get corresponding bits from 2 bytes
    bool hiBit = Utils::getBit(hiByte, 7 - pxOffset);
    bool loBit = Utils::getBit(loByte, 7 - pxOffset);
    Palette pal(getBackgroundPalette(tileAttr & 7));
    
    // pixel color index
    // bit of (lo/hi) byte is the (lo/hi) bit of the 2-bit color palette index
    u8 pxIdx = static_cast<u8>(hiBit << 1 | loBit);
    // 0 -> highest priority
    // pxIdx == 0 -> lowest priority
    u8 priority = (pxIdx == 0) ? 2 : 1;
    if(Utils::getBit(tileAttr, 7))
        priority = 0;
    uint32_t texIdx = texX + texY * tex.getWidth();
    tex.setPixelColor(texIdx, pal[pxIdx]);
    tex.setPixelPriority(texIdx, priority);
}

Texture PPU::getLayer(Layer l) const {
    Texture tex(256, 256);
    for(uint32_t y = 0; y < 256; ++y) {
        for(uint32_t x = 0; x < 256; ++x) {
            renderLayerPixel(tex, l, x, y, x, y);
        }
    }
    return tex;
}

std::array<u8, 32*32> PPU::getRawBackground() {
    u16 bgMap = Utils::getBit(LCDC_, 3) ? 0x9C00 : 0x9800;
    std::array<u8, 32*32> bg {};
    for(u16 i = 0; i < 32*32; ++i)
        bg[i] = memory_.read(bgMap + i);
    return bg;
}

Texture PPU::getFrameBuffer(bool withBg, bool withWin, bool withSprt) const {
    Texture frame = withBg ? getLayer(Layer::Background) : Texture(256, 256);
    if(withWin) {
        Texture window(getLayer(Layer::Window));
        // copy window layer onto frame buffer in correct position
        for(u16 y = 0; y < 256; ++y) {
            for(u16 x = 0; x < 256; ++x) {
                u16 wx = WX_ - 7 - SCX_ + x;
                u16 wy = WY_ - SCY_ + y;
                if(wx > 0xFF || wy > 0xFF || wx >= SCX_+160 || wy >= SCY_+144)
                    continue;
                uint32_t wIdx = wx + wy * 256;
                frame.setPixelColor(wIdx, window.getPixelColor(x+y*256));
            }
        }
    }
    return frame;
}

void PPU::setMode(u8 mode) {
    if(mode < 4)
        STAT_ &= (0b11111100 | mode);
    else
        Utils::error("Unhandled mode value in PPU!");
}

void PPU::loadSprites() {
    u8 i = 0;
    // sprites are 4 bytes each
    for(u16 adr = 0xFE00; adr < 0xFEA0; adr += 4, ++i) {
        sprites_[i].y = memory_.read(adr);
        sprites_[i].x = memory_.read(adr+1);
        sprites_[i].tile = memory_.read(adr+2);
        sprites_[i].attribute = memory_.read(adr+3);
        sprites_[i].id = i;
    }
}

void PPU::orderSprites(std::array<Sprite, 10>& s) const {
    std::sort(s.begin(), s.end(), [](const Sprite& a, const Sprite& b) {
        return (a.x == b.x) ? (a.id > b.id) : (a.x > b.x);
    });
}

Palette PPU::getBackgroundPalette(u8 idx) const {
    Palette pal {};
    for(u8 i = 0; i < 4; ++i)
        pal[i] = dmgPalette[((BGP_ >> i*2) & 3)];
    return pal;
}

Palette PPU::getSpritePalette(u8 idx) const {
    Palette pal {};
    u8 obp = idx ? OBP1_ : OBP0_;
    for(u8 i = 1; i < 4; ++i)
        pal[i] = dmgPalette[((obp >> i*2) & 3)];
    pal[0] = dmgPalette[0]; // 00 in sprite palette is always transparent
    return pal;
}

void PPU::renderSpriteLine(Texture& tex) {
    const u16 tileData = 0x8000;
    // 0 -> 8 x 8, 1 -> 8 x 16
    const u8 spriteH = Utils::getBit(LCDC_, 2) ? 16 : 8;
    u8 spritesDrawn = 0;
    // sprite priority list
    // get first 10 sprites and reorder
    // so that lowest priority is drawn first
    std::array<Sprite, 10> orderedSprites {};
    for(u8 i = 0; i < 40; ++i) {
        Sprite& s = sprites_[i];
        // sprite is off screen, so skip
        if(s.y == 0 || s.y >= 160)
            continue;
        // stop drawing sprites if 10 already on screen
        if(spritesDrawn == 10)
            break;
        // line number relative to sprite start
        u8 ln = LY_ - (s.y-16);
        // ignore sprite if no part of it is on this line
        if(ln > spriteH - 1 || LY_ < s.y-16)
            continue;
        orderedSprites[spritesDrawn] = s;
        ++spritesDrawn;
    }
    // draw in OAM memory order
    orderSprites(orderedSprites);
    for(Sprite s : orderedSprites) {
        u8 tileIdx;
        // line number relative to start of sprite
        u8 ln = LY_ - (s.y-16);
        bool yFlip = Utils::getBit(s.attribute, 6);
        // lower half of 8x16 sprite
        if(ln > 7) {
            tileIdx = yFlip ? s.tile & 0xFE : s.tile | 0x01;
        }
        else {
            // upper half of 8x16 sprite
            if(spriteH == 16) {
                tileIdx = yFlip ? s.tile | 0x01 : s.tile & 0xFE;
            }
            // 8x8 sprite
            else {
                tileIdx = s.tile;
            }
        }

        u16 adr = tileData+tileIdx*16;

        if(yFlip)
            adr += (7 - (ln % 8)) * 2;
        else
            adr += (ln % 8) * 2;

        u8 loByte = readVram(0, adr);
        u8 hiByte = readVram(0, adr+1);
        u8 palIdx = Utils::getBit(s.attribute, 4);
        Palette pal = getSpritePalette(palIdx);
        bool objPrio = Utils::getBit(s.attribute, 7);

        for(u8 px = 0; px < 8; ++px) {
            u8 x = s.x - 8 + px;
            bool onScreen = x < 160;
            u8 bgPriority = tex.getPixelPriority(x);
            if(onScreen &&
                    ((not objPrio && bgPriority > 0)
                    || (objPrio < bgPriority))) {
                bool xFlip = Utils::getBit(s.attribute, 5);
                u8 offset = xFlip ? px : 7 - px;
                bool hiBit = Utils::getBit(hiByte, offset); 
                bool loBit = Utils::getBit(loByte, offset); 
                u8 p = static_cast<u8>(hiBit << 1 | loBit);
                // draw only if not transparent
                if (p != 0)
                    tex.setPixelPriority(x, pal[p]);
            }
        }
    }
}

void PPU::renderScanline() {
    Texture tex {160, 1};
    // STOP mode: if LCD is on -> set to all white, else all black
    Utils::warning("render scanline!");
    if(not cpu_.isStopped()) {
        Color c = isEnabled() ? 0xFFFF : 0;
        tex.fill(c);
    }
    else {
        if(LCDC_ & 1) {
            Utils::warning("render background!");
            renderLayerLine(tex, Layer::Background);
            if(Utils::getBit(LCDC_, 5))
                renderLayerLine(tex, Layer::Window);
        }
        else {
            Utils::warning("fill with whatever!");
            Color c(0xFFF);
            tex.fill(c);
        }
        if(Utils::getBit(LCDC_, 1))
            renderSpriteLine(tex);
    }
    renderer_->drawTexture(tex, 0, LY_);
}

// OAM_SCAN mode 2 -> 3
void PPU::handleOamScan() {
    if(cycles_ < 80)
        return;
    cycles_ -= 80;
    std::cout << "reading from VRAM" << std::endl;
    loadSprites();
    // entering VRAM_READ
    setMode(3);
}

// VRAM_READ mode 3 -> 0
void PPU::handleVramRead() {
    std::cout << "reading from VRAM" << std::endl;
    if(cycles_ < 172)
        return;
    cycles_ -= 172;
    if(not isRenderer_)
        return;
    renderScanline();
    // entering HBlank
    setMode(0);
}

// HBLANK mode 0 -> 1, 0 -> 2
void PPU::handleHBlank() {
    if(cycles_ < 204)
        return;

    cycles_ -= 204;
    ++LY_;
    // enter VBlank mode 1 
    if(LY_ == 144) {
        windowLine_ = 0;
        setMode(1);
        ic_.requestInterrupt(InterruptController::Type::VBlank);
        if(not isRenderer_) {
            return;
        }
        renderer_->showScreen();
    }
    // enter OAM_SCAN mode 2
    else {
        setMode(2);
    }
}

// VBlank
void PPU::handleVBlank() {
    if(cycles_ < 456)
        return;
    cycles_ -= 456;

    ++LY_;
    // enter OAM_SCAN mode 2
    if(LY_ <= 153)
        return;
    setMode(2);
    LY_ = 0;
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
        // STAT interrupt is requested only if signal goes from 0 to 1
        if(not statSignal_)
            ic_.requestInterrupt(InterruptController::Type::LCD);
        statSignal_ = true;
    }
    else {
        statSignal_ = false;
    }
}

Palette PPU::dmgPalette = {{0xFFFF, 0x56B5, 0x294A, 0}};
