#include "GraphicTypes.hpp"

Texture::Texture(u16 w, u16 h) 
    : width_(w)
    , height_(h)
    , data_(w*h)
    , pixelIds_(w*h) {
    for(u8& p : pixelIds_) {
        p = 0xFF;
    }
}

void Texture::fill(const Color& c) {
    for(size_t i = 0; i < width_ * height_; ++i) {
        data_[i] = c;
        pixelIds_[i] = 0;
    }
}


