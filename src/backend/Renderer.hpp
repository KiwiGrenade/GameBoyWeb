#include "PPU.hpp"

class Texture;

class Renderer {
public:
    virtual void drawTexture(const Texture& tex, u16 x, u16 y) = 0;
    virtual void showScreen() = 0;
};
