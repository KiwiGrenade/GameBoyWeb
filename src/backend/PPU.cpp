#include "PPU.hpp"
#include "utils.hpp"

#include "Renderer.hpp"
#include <string>
#include <cmath>
#include <algorithm>

PPU::PPU(InterruptController &ic,
         CPUClock &cpuClock,
         Renderer *r)
        : ic_(ic),
          cpuClock_{cpuClock},
          renderer_{r} {}

void PPU::reset() {
    cycles_ = 0;
    scanline_ = 0;
    LCDC_ = 0x90;
    STAT_ = 0x00;
    SCY_ = 0;
    SCX_ = 0;
    LY_ = 0;
    LYC_ = 0;
    BGP_ = 0xFC;
    OBP0_ = 0xFF;
    OBP1_ = 0xFF;
    WY_ = 0;
    WX_ = 0;
    sprites_ = {};
    isStatInterrupt_ = false;
    oam_ = {};
}

u8 PPU::readOam(u16 adr) const {
    if (adr >= oam_.size())
        Utils::warning("Trying to READ from wrong OAM index!");
    return oam_[adr];
}

void PPU::writeOam(u8 b, u16 adr) {
    if (adr >= oam_.size())
        Utils::warning("Trying to WRITE to wrong OAM index!");
    oam_[adr] = b;
}

u8 PPU::readVram(u16 adr) const {
    adr -= 0x8000;
    if (adr >= vram_.getSize())
        Utils::warning("Trying to READ from wrong VRAM index!");
    return vram_.read(0, adr);
}

void PPU::writeVram(u8 b, u16 adr) {
    adr -= 0x8000;
    if (adr >= vram_.getSize())
        Utils::warning("Trying to WRITE to wrong VRAM index!");
    vram_.write(b, 0, adr);
}

void PPU::update(size_t cycles) {
    // PPU operates on 4.194 MHz clock, 1 clock = 1/4 cycle
    cycles_ += cycles;
    if (not isEnabled()) // bit 7
    {
        cycles_ = 0;
        LY_ = 0;
        setMode(0);
        return; // don't execute if master bit is off
    }

    checkStatus();

    switch (getMode()) // bit 0-1
    {
        // mode 2: scan for OAM sprites
        case 2:
            oamScanMode();
            break;
            // OAM/VRAM read
            // end of mode 3 = end of scan line
        case 3:
            vramReadMode();
            break;
            // HBLANK
        case 0:
            hblankMode();
            break;
            // VBLANK
        case 1:
            vblankMode();
            break;
    }
}

void PPU::writeReg(u8 b, u16 adr) {
    switch (adr) {
        case 0xFF40:
            LCDC_ = b;
            break;
        case 0xFF41: {
            // lower 3 bits of STAT are read only,
            // only write to upper 4 bits
            // clear upper 4 bits
            STAT_ &= ~0xF8;
            // set upper 4 bits
            STAT_ |= b & 0xF8;
        }
            break;
        case 0xFF42:
            SCY_ = b;
            break;
        case 0xFF43:
            SCX_ = b;
            break;
        case 0xFF44:
            LY_ = b;
            break;
        case 0xFF45:
            LYC_ = b;
            break;
            // FF46: DMA transfer
        case 0xFF47:
            BGP_ = b;
            break;
        case 0xFF48:
            OBP0_ = b;
            break;
        case 0xff49:
            OBP1_ = b;
            break;
        case 0xff4A:
            WY_ = b;
            break;
        case 0xff4B:
            WX_ = b;
            break;
        default:
            break;
            /*throw qtboy::Exception("Attempted to write invalid PPU register.",*/
            /*                         __FILE__, __LINE__);*/
    }
}

u8 PPU::readReg(u16 adr) {
    u8 b = 0xFF;
    switch (adr) {
        case 0xFF40:
            b = LCDC_;
            break;
        case 0xFF41:
            b = STAT_;
            break;
        case 0xFF42:
            b = SCY_;
            break;
        case 0xFF43:
            b = SCX_;
            break;
        case 0xFF44:
            b = LY_;
            break;
        case 0xFF45:
            b = LYC_;
            break;
            // FF46: DMA transfer
        case 0xFF47:
            b = BGP_;
            break;
        case 0xFF48:
            b = OBP0_;
            break;
        case 0xFF49:
            b = OBP1_;
            break;
        case 0xFF4A:
            b = WY_;
            break;
        case 0xFF4B:
            b = WX_;
            break;
        default:
            break;
            /*throw qtboy::Exception("Attempted to read invalid PPU register.",*/
            /*                         __FILE__, __LINE__);*/
    }
    return b;
}

Texture PPU::getFramebuffer(bool with_bg, bool with_win,
                             bool with_sprites) const {
    Texture frame = with_bg ? getLayer(Layer::Background) : Texture(256, 256);
    if (with_win) {
        Texture window(getLayer(Layer::Window));
        // copy window layer onto frame buffer layer in the correct position
        for (u16 y = 0; y < 256; ++y) {
            for (u16 x = 0; x < 256; ++x) {
                u16 wx = WX_ - 7 - SCX_ + x;
                u16 wy = WY_ - SCY_ + y;
                if (wx > 0xff || wy > 0xff
                    || wx >= SCX_ + 160 || wy >= SCY_ + 144)
                    continue;
                unsigned wi = wx + wy * 256;
                frame.set_pixel(wi, window.pixel(x + y * 256));
            }
        }
    }
    return frame;
}

Texture PPU::getTile(u16 i) const {
    Texture tex(8, 8);
    u16 tile_base = 0x8000;
    for (u8 byte = 0; byte < 16; byte += 2) {
        u8 lo_byte = readVram(byte + tile_base + i * 16);
        u8 hi_byte = readVram(byte + tile_base + i * 16 + 1);
        for (u8 px = 0; px < 8; ++px) {
            bool hi_bit = hi_byte & 1 << (7 - px);
            bool lo_bit = lo_byte & 1 << (7 - px);
            u8 color_i = static_cast<u8>(hi_bit << 1 | lo_bit);
            Color c = dmgPalette_[color_i];
            u8 p = (byte / 2) * 8 + px;
            tex.set_pixel(p, c);
            tex.set_pixel_priority(p, 0); // irrelevant
        }
    }
    return tex;
}

Texture PPU::getLayer(Layer l) const {
    Texture tex(256, 256);
    for (unsigned y = 0; y < 256; ++y) {
        for (unsigned x = 0; x < 256; ++x) {
            renderLayerPixel(tex, l, x, y, x, y);
        }
    }
    return tex;
}

std::array<u8, 32 * 32> PPU::getRawBg() {
    u16 bg_map = ((LCDC_ & (1 << 3)) ? 0x9c00 : 0x9800);
    std::array<u8, 32 * 32> raw{};
    for (int i = 0; i < 32 * 32; ++i)
        raw[i] = readVram(bg_map + i);
    return raw;
}

void PPU::renderScanline() {
    Texture tex{160, 1};
    // STOP mode: if LCD is on, set to all white, if off, all black
    if (false && cpuClock_.isStopped_) {
        Color c = (LCDC_ & 0x80) ? 0xffff : 0;
        tex.fill(c);
    } else {
        if (LCDC_ & 1) // bg/window enable
        {
            renderLayerLine(tex, PPU::Layer::Background);
            if (LCDC_ & 1 << 5) // window display enable
                renderLayerLine(tex, PPU::Layer::Window);
        }
            // background and window appear white if lcdc bit 0 is cleared
        else {
            Color c(0xffff);
            tex.fill(c);
        }
        if (LCDC_ & 1 << 1) // OBJ display enable
            renderSpriteLine(tex);
    }
    renderer_->drawTexture(tex, 0, LY_);
}

void PPU::renderLayerLine(Texture &tex, PPU::Layer layer) {
    // don't draw windo if the current line isn't a window line
    if (layer == PPU::Layer::Window && LY_ < WY_)
        return;
    // current y-coordinate of the 256x256 layer
    u8 y = 0;
    if (layer == PPU::Layer::Background)
        y = LY_ + SCY_;
    else if (layer == PPU::Layer::Window)
        y = scanline_;
    // draw each pixel in the scanline
    u8 window_pxs_drawn = 0;
    for (u8 x_px = 0; x_px < 160; ++x_px) {
        // don't draw window if it doesn't appear yet
        if (layer == PPU::Layer::Window) {
            // x: window could appear later in the line
            if (x_px < WX_ - 7)
                continue;
                // y: window does not appear at all in this line
            else if (WX_ < 7 || WX_ > 166 || WY_ > 143)
                return;
        }
        // current x-coordinate of the 256x256 layer
        u8 x = (layer == PPU::Layer::Background)
                    ? x_px + SCX_
                    : x_px - (WX_ - 7);
        renderLayerPixel(tex, layer, x, y, x_px, 0);
        ++window_pxs_drawn;
    }
    if (layer == PPU::Layer::Window && window_pxs_drawn != 0)
        ++scanline_;
}

// render the layer pixel at (tex_x,tex_y) with the color at (x, y) in VRAM
void PPU::renderLayerPixel(Texture &tex, PPU::Layer layer,
                             u8 x, u8 y,
                             u8 tex_x, u8 tex_y) const {
    u16 tile_map = 0x9800;
    if (layer == PPU::Layer::Background) {
        tile_map = ((LCDC_ & (1 << 3)) ? 0x9c00 : 0x9800);

    } else if (layer == PPU::Layer::Window) {
        tile_map = ((LCDC_ & (1 << 6)) ? 0x9c00 : 0x9800);
    }
    // tile data offset in VRAM
    u16 tile_base = (LCDC_ & 1 << 4) ? 0x8000 : 0x9000;
    // x-index into the tile BG map (tile_map) of the tile to draw
    u8 tile_x = x >> 3; // tiles are 8 px wide
    // y tile index into the tile map
    u8 tile_y = y >> 3; // tiles are 8 px tall
    // index into tile BG map of the tile to draw
    u16 tile_i = (tile_y * 32) + tile_x + tile_map;
    // in VRAM bank 1
    u8 tile_attribute = 0;
    // index of the tile in tile data of the tile to draw
    u8 tile_data_i = readVram(tile_i);
    // bank containing the tile data
    u8 bank = (tile_attribute & 1 << 3) ? 1 : 0;
    // location to read tile data from
    u16 adr = 0;
    // 0x9000 base uses signed addressing
    if (tile_base == 0x9000) {
        // starting address of the data for the tile (1 tile is 16 bytes)
        adr = tile_base + static_cast<int8_t>(tile_data_i) * 16;
    }
        // 0x8000 uses unsigned addressing
    else {
        // starting address of the data for the tile (1 tile is 16 bytes)
        adr = tile_base + tile_data_i * 16;
    }
    // point address to the right byte in the tile data based on y pos
    if (tile_attribute & 1 << 6) // vertical flip
        adr += (7 - (y % 8)) * 2;
    else
        adr += (y % 8) * 2;
    // get two bytes (one row of pixels) from the tile data
    u8 lo_byte = readVram(adr);
    u8 hi_byte = readVram(adr + 1);
    // get the pixel offset of the tile (0-7) (tiles are 8x8)
    // take into account horizontal flip
    u8 px_offset = (x % 8);
    if (tile_attribute & 1 << 5)
        px_offset = 7 - (x % 8);
    // get 2 corresponding bits, 1 from each byte
    // most significant bits of both bytes represent color of first
    // (or last) pixel
    bool hi_bit = (hi_byte & 1 << (7 - px_offset));
    bool lo_bit = (lo_byte & 1 << (7 - px_offset));
    Palette pal(getBgPalette(tile_attribute & 7));
    // get pixel color index
    // bit of hi byte is the hi bit of the 2-bit color index in the palette
    // bit of lo byte is the lo bit of the 2-bit color index in the palette
    u8 px_i = static_cast<u8>(hi_bit << 1 | lo_bit);
    // 0 = highest priority (appears above everything else)
    // px_index == 0 has lowest priority
    u8 priority = (px_i == 0) ? 2 : 1;
    if (tile_attribute & 1 << 7) // ensure highest priority
        priority = 0;
    // index into texture
    unsigned tex_i = tex_x + tex_y * tex.width();
    tex.set_pixel(tex_i, pal[px_i]);
    tex.set_pixel_priority(tex_i, priority);
}

void PPU::renderSpriteLine(Texture &tex) {
    const u16 tile_data = 0x8000;
    // 0=8x8, 1=8x16
    const u8 sprite_h = LCDC_ & 1 << 2 ? 16 : 8;
    u8 sprites_drawn = 0;
    // sprite priority list: get first 10 sprites, then reorder so the lowest
    // priority is drawn first
    std::array<Sprite, 10> ordered_sprites{};
    for (u8 i = 0; i < 40; ++i) {
        Sprite &s = sprites_[i];
        // skip if sprite is offscreen
        if (s.y == 0 || s.y >= 160)
            continue;
        // stop drawing sprites if more than 10 are already on this scanline
        if (sprites_drawn == 10)
            break;
        // line number relative to start of sprite
        u8 ln = LY_ - (s.y - 16);
        // ignore sprite if no part of it falls on this line
        if (ln > sprite_h - 1 || LY_ < s.y - 16)
            continue;
        ordered_sprites[sprites_drawn] = s;
        ++sprites_drawn;
    }
    // first object in OAM has highest priority, so draw first
    orderSprites(ordered_sprites);
    for (Sprite s: ordered_sprites) {
        u8 tile_i;
        // line number relative to start of sprite
        u8 ln = LY_ - (s.y - 16);
        bool y_flip = s.attribute & 1 << 6;
        if (ln > 7) // lower half of an 8x16 sprite
        {
            tile_i = (y_flip)
                     ? s.tile & 0xfe
                     : s.tile | 0x01;
        } else {
            if (sprite_h == 16) // upper half of an 8x16 sprite
            {
                tile_i = (y_flip)
                         ? s.tile | 0x01
                         : s.tile & 0xfe;
            } else // 8x8 sprite
            {
                tile_i = s.tile;
            }
        }
        u16 adr = tile_data + tile_i * 16;
        if (y_flip)
            adr += (7 - (ln % 8)) * 2;
        else
            adr += (ln % 8) * 2;
        u8 low_byte = readVram(adr);
        u8 high_byte = readVram(adr + 1);
        u8 pal_idx = s.attribute & 1 << 4;
        Palette pal = getSpritePalette(pal_idx);
        bool ob_priority = s.attribute & 1 << 7;
        for (u8 px = 0; px < 8; ++px) {
            u8 x = s.x - 8 + px;
            bool on_screen = (x < 160 && x > 0);
            u8 bg_priority = tex.pixel_priority(x);
            // only draw pixel if on screen and if priority is 0 and bg pixel priority
            // IS NOT 0, or if priority is 1
            // and bg pixel is >=2 (transparent)
            if (on_screen
                && ((ob_priority == 0 && bg_priority > 0)
                    || (ob_priority < bg_priority))) {
                bool x_flip = s.attribute & 1 << 5;
                bool hi_bit = (high_byte & 1 << (x_flip ? px : 7 - px));
                bool lo_bit = (low_byte & 1 << (x_flip ? px : 7 - px));
                u8 p = static_cast<u8>(hi_bit << 1 | lo_bit);
                if (p != 0) // only draw if not transparent
                    tex.set_pixel(x, pal[p]);
            }
        }
    }
}

std::array<Texture, 40> PPU::getSprites() const {
    std::array<Texture, 40> out{};
    int i = 0;
    for (const Sprite &s: sprites_) {
        u8 bank = s.attribute & 1 << 3 ? 1 : 0;
        out[i++] = getTile(s.tile);
    }
    return out;
}

void PPU::orderSprites(std::array<Sprite, 10> &s) const {
    // lower X priority = higher priority, tie breaker: lower OAM
    std::sort(s.begin(), s.end(), [](const Sprite &a, const Sprite &b) {
        return (a.x == b.x) ? (a.id > b.id) : (a.x > b.x);
    });
}

Palette PPU::getBgPalette(u8 idx) const {
    Palette pal{};
    for (u8 i{0}; i < 4; ++i)
        pal[i] = dmgPalette_[((BGP_ >> i * 2) & 3)];
    return pal;
}

Palette PPU::getSpritePalette(u8 idx) const {
    Palette pal{};
    u8 obp = idx ? OBP1_ : OBP0_;
    for (u8 i{1}; i < 4; ++i)
        pal[i] = dmgPalette_[((obp >> i * 2) & 3)];
    pal[0] = dmgPalette_[0]; // 00 in sprite palette is always transparent
    return pal;
}

void PPU::loadSprites() {
    u8 i = 0;
    for (u16 adr = 0; adr < 0xa0; adr += 4) // sprites are 4 bytes
    {
        sprites_[i].y = oam_[adr];
        sprites_[i].x = oam_[adr + 1];
        sprites_[i].tile = oam_[adr + 2];
        sprites_[i].attribute = oam_[adr + 3];
        sprites_[i].id = i;
        ++i;
    }
}

void PPU::setMode(u8 mode) {
    switch (mode) {
        case 3:
            Utils::setBit(STAT_, 1, true);
            Utils::setBit(STAT_, 0, true);
            break;
        case 2:
            Utils::setBit(STAT_, 1, true);
            Utils::setBit(STAT_, 0, false);
            break;
        case 1:
            Utils::setBit(STAT_, 1, false);
            Utils::setBit(STAT_, 0, true);
            break;
        case 0:
            Utils::setBit(STAT_, 1, false);
            Utils::setBit(STAT_, 0, false);
            break;
        default:
            Utils::error("Trying to enter undefined mode!");
    }
}

// oamScan mode 2
void PPU::oamScanMode() {
    if (cycles_ >= 80) {
        cycles_ -= 80;
        loadSprites();
        setMode(3);
    }
}

// readVram mode 3
void PPU::vramReadMode() {
    if (cycles_ >= 172) {
        cycles_ -= 172;
        if (renderer_)
            renderScanline();
        // enter hblank
        setMode(0);
    }
}

// HBLANK mode 0
void PPU::hblankMode() {
    if (cycles_ >= 204) {
        cycles_ -= 204;
        ++LY_;
        if (LY_ == 144) {
            // enter vblank
            scanline_ = 0;
            setMode(1);
            ic_.requestInterrupt(InterruptController::VBlank);
            if (renderer_)
                renderer_->showScreen();
        } else {
            // enter oamScan
            setMode(2);
        }
    }
}

// VBLANK mode 1
void PPU::vblankMode() {
    if (cycles_ >= 456) {
        cycles_ -= 456;
        ++LY_;
        if (LY_ > 153) {
            // restart scanning modes
            setMode(2);
            LY_ = 0;
        }
    }
}

void PPU::checkStatus() {
    int mod = getMode();
    bool lyc_comp = (LY_ == LYC_ && Utils::getBit(STAT_, 6));
    bool vbl_chk = (mod == 0 && Utils::getBit(STAT_, 3));
    bool oam_chk = (mod == 2 && Utils::getBit(STAT_, 5));
    bool hbl_chk = (mod == 1
                    && ((STAT_ & 1 << 4) || (STAT_ & 1 << 5)));
        Utils::setBit(STAT_, 2, LY_ == LYC_);
    if (lyc_comp || vbl_chk || oam_chk || hbl_chk) {
        // STAT interrupt is only request when signal goes from 0->1
        if (!isStatInterrupt_)
            ic_.requestInterrupt(InterruptController::LCD);
        isStatInterrupt_ = true;
    } else {
        isStatInterrupt_ = false;
    }
}

Palette PPU::dmgPalette_ = {{0xffff, 0x56b5, 0x294a, 0}};
