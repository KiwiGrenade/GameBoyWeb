#include "QtRenderer.hpp"
#include "utils.hpp"

#include <cmath>
#include <mutex>

QtRenderer::QtRenderer(u16 w, u16 h, QObject *parent)
    : QObject {parent}
    , width_(w)
    , height_(h)
    , buf_(2*w*h) 
{}


void QtRenderer::clear() {
    buf_.clear();
    buf_.resize(width_ * height_ * 4);
}

QImage QtRenderer::getImage() const {
    const std::lock_guard<std::mutex> lock(buf_mutex_);
    QImage img(buf_.data(), width_, height_, QImage::Format_RGB555);
    return img;
}

QPixmap QtRenderer::getPixmap() const {
    return QPixmap::fromImage(getImage());
}

void QtRenderer::drawTexture(const Texture& tex, u16 xOffset, u16 yOffset) {
    const u16 w { tex.getWidth() };
    const u16 h { tex.getHeight() };
    {
        const std::lock_guard<std::mutex> lock(buf_mutex_);
        for(uint32_t i = 0; i < w * h; ++i) {
            int x = xOffset + i % w;
            int y = yOffset + i / w;
            // prevents wrapping
            if(x > width_ || y > height_)
                continue;
            int j = y * width_ + x;
            // represent in RGB555
            Color c = tex.getPixelColor(i);
            buf_[2*j] = c & 0xFF;
            buf_[2*j+1] = c >> 8;
        }
    }
}
