#pragma once

#include "GraphicTypes.hpp"
#include "CPUClock.hpp"
#include "InterruptController.hpp"
#include "Ram.hpp"

class Renderer;

class PPU {
public:

    enum class Layer {
        Background, Window, Sprite
    };

    PPU(InterruptController &ic,
        CPUClock &cpuClock,
        Renderer *r = nullptr);

    void reset();
    void step(size_t cycles);

    // getters
    inline int getCycles() const { return cycles_; };
    inline u8 getMode() const { return STAT_ & 3; }
    inline bool isEnabled() const { return LCDC_ & 0x80; }
    u8 readReg(u16 adr);
    u8 readOam(u16 addr) const;
    u8 readVram(u16 adr) const;

    // setters
    inline void setRenderer(Renderer *r) { renderer_ = r; }
    void setMode(u8 mode);
    void writeReg(u8 b, u16 adr);
    void writeOam(u8 byte, u16 addr);
    void writeVram(u8 b, u16 adr);

private:
    // getters
    Palette getBgPalette(u8 idx) const;
    Palette getSpritePalette(u8 idx) const;
    Texture getFramebuffer(bool bg = true, bool window = true,
                            bool sprites = true) const;
    Texture getTile(u16 i) const;
    std::array<Texture, 40> getSprites() const;
    Texture getLayer(Layer) const;
    std::array<u8, 32 * 32> getRawBg();

    // rendering
    void renderScanline();
    void renderLayerLine(Texture &tex, Layer l);
    // (x,y): VRAM tilemap pixel coordinates (to get)
    // (tex_x, tex_y): pixel to draw in Texture
    void renderLayerPixel(Texture &tex, Layer l, u8 x, u8 y,
                            u8 tex_x, u8 tex_y) const;
    void renderSpriteLine(Texture &tex);
    void orderSprites(std::array<Sprite, 10> &s) const;
    void loadSprites();

    void oamScanMode(); // mode 2
    void vramReadMode(); // mode 3
    void hblankMode(); // mode 0
    void vblankMode(); // mode 1
    void checkStatus();

private:
    InterruptController &ic_;
    CPUClock &cpuClock_;
    Renderer *renderer_;

    // memory
    std::array<u8, 0xA0> oam_{};
    VRam vram_{1};

    int cycles_{0};
    u8 scanline_{0};
    // 0xFF40 - LCD control
    u8 LCDC_{0x90};
    // 0xFF41 - LCD status
    u8 STAT_{0x00};
    // 0xFF42 - Scroll Y
    u8 SCY_{0};
    // 0xFF43 - Scroll X
    u8 SCX_{0};
    // 0xFF44 - Scanline Y
    u8 LY_{0};
    // 0xFF45 - Scanline Compare
    u8 LYC_{0};
    // 0xFF47 - Background pallet
    u8 BGP_{0xfc};
    // 0xFF48 - Object Background pallet 0
    u8 OBP0_{0xff};
    // 0xFF49 - Object Background pallet 1
    u8 OBP1_{0xff};
    // 0xFF4A - Window Y
    u8 WY_{0};
    // 0xFF4B - Window X
    u8 WX_{0};
    std::array<Sprite, 40> sprites_{};
    bool isStatInterrupt_{false}; // for activating STAT interrupt
    static Palette dmgPalette_;
};


