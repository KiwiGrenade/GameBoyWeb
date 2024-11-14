#pragma once

#include "utils.hpp"
#include "InterruptController.hpp"
#include "GraphicTypes.hpp"
#include "Ram.hpp"
#include <cstdint>

class Renderer;
class Processor;
class Memory;

class PPU {
public:
    PPU(Processor& cpu, Renderer* r = nullptr);
    void update(size_t cycles);
    void reset();
    u8 read(u16 addr);
    void write(u8 byte, u16 addr);
    inline bool isEnabled() { return Utils::getBit(LCDC_, 7); };
    Texture getTile(uint16_t i) const;
    inline u8 getMode() { return STAT_ & 3; };
    void setRenderer(Renderer *r) {
        renderer_ = r;
        isRenderer_ = true;
    };
    u8 readVram(u8 bank, u16 addr) const;
    void writeVram(u8 byte, u8 bank, u16 addr);
    u8 readOam(u16 addr) const;
    void writeOam(u8 byte, u16 addr);

protected:
    enum class Layer { Background, Window, Sprite };

    void checkStatus();
    void setMode(u8 model);
    // getters
    Texture getLayer(Layer l) const;
    Texture getFrameBuffer(bool withBg, bool withWin, bool withSprt) const;
    std::array<u8, 32*32> getRawBackground();
    Palette getSpritePalette(u8 idx) const;
    Palette getBackgroundPalette(u8 idx) const;
    // rendering
    void renderLayerPixel(
        Texture& tex,
        Layer layer,
        u8 x,
        u8 y,
        u8 texX,
        u8 texY) const;
    void renderLayerLine(Texture& tex, Layer layer);
    void renderSpriteLine(Texture& tex);
    void renderScanline();
    // sprites
    void loadSprites();
    void orderSprites(std::array<Sprite, 10> &s) const;
    // handlers
    void handleOamScan();
    void handleVramRead();
    void handleHBlank();
    void handleVBlank();
    void drawwindowLine();
    void dmaTransfer(u8 b);

    // GB modules
    Processor& cpu_;
    Renderer *renderer_;
    
    // memory
    std::array<u8, 0xA0> oam_ {};
    VRam vram_ {1};
    
    std::array<Sprite, 40> sprites_ {};
    static Palette dmgPalette;
    int cycles_ = 0;
    u16 windowLine_ = 0;
    bool statSignal_ = false;
    bool isRenderer_ = false;
    
    // registers
    u8 LCDC_ = 0x91;   // 0xFF40
    u8 STAT_ = 0x81;   // 0xFF41
    u8 SCY_;    // 0xFF42
    u8 SCX_;    // 0xFF43
    u8 LY_;     // 0xFF44
    u8 LYC_;    // 0xFF45
    u8 BGP_ = 0xFC;    // 0xFF47
    u8 OBP0_ = 0xFF;   // 0xFF48
    u8 OBP1_ = 0xFF;   // 0xFF49
    u8 WY_;     // 0xFF4A
    u8 WX_;     // 0xFF4B
};
