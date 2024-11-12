#pragma once

#include "utils.hpp"
#include "InterruptController.hpp"
#include "GraphicTypes.hpp"
#include <cstdint>

class Renderer;
class CPU;
class Memory;

class PPU {
public:
    PPU(InterruptController& ic, Memory& memory, const CPU& cpu, Renderer* r = nullptr);
    void update(uint32_t cycles);
    void reset();
    u8 read(u16 addr);
    void write(u8 byte, u16 addr);
    inline bool isEnabled() { return Utils::getBit(LCDC_, 7); };
    Texture getTile(uint16_t i) const;

protected:
    enum class Layer { Background, Window, Sprite };

    void checkStatus();
    void setMode(u8 model);
    u8 readVram(u8 bank, u16 addr) const;
    // getters
    inline u8 getMode() { return STAT_ & 3; };
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
    InterruptController& ic_;
    Memory& memory_;
    const CPU& cpu_;
    Renderer *renderer_;
    
    std::array<Sprite, 40> sprites_ {};
    static Palette dmgPalette;
    uint32_t cycles_;
    u16 windowLine_;
    bool statSignal_;
    bool isRenderer_;
    
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
