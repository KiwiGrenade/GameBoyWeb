#pragma once

#include <cstdint>
#include <array>
#include <vector>

#include "utils.hpp"

typedef u16 Color;
typedef std::array<Color, 4> Palette;

class Texture {
public:
    Texture(u16 w = 1, u16 h = 1);

    void fill(const Color& c);
    inline void setPixelColor(uint64_t i, const Color &c) { data_[i] = c; };
    Color getPixelColor(uint64_t i) { return data_[i]; };
    void setPixelPriority(uint64_t i, u8 x) { pixelIds_[i] = x; };
    u8 getPixelPriority(uint64_t i) { return pixelIds_[i]; };
    inline u16 getHeight() { return height_; };
    inline u16 getWidth() { return width_; };

private:
    u16 height_;
    u16 width_;
    std::vector<Color> data_;
    std::vector<u8> pixelIds_;
};

struct Sprite {
    u8 y, x, tile, attribute, line, id;
};
