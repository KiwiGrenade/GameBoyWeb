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

    void fill(const Color &c);
    inline void set_pixel(uint64_t i, const Color &c) { data_[i] = c; };
    inline Color pixel(uint64_t i) const { return data_[i]; };
    inline void set_pixel_priority(uint64_t i, u8 x) { pixelIds_[i] = x; };
    inline u8 pixel_priority(uint64_t i) const { return pixelIds_[i]; };
    inline u16 height() const { return height_; };
    inline u16 width() const { return width_; };

private:
    u16 height_;
    u16 width_;
    std::vector <Color> data_;
    std::vector <u8> pixelIds_;
};

struct Sprite {
    u8 y, x, tile, attribute, line, id;
};
