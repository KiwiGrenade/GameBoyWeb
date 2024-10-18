#pragma once

#include "utils.hpp"

#include <QByteArray>

class Cartridge {
public:
    Cartridge() = default;
    Cartridge(const QByteArray& arr);
    
    inline u8 read(const u16 addr) { return rom_[addr]; };
    inline std::string getTitle() { return title_; };

    static constexpr u16 romSize_ = 0x8000;
private:
    std::array<u8, romSize_> extractROM(const QByteArray& arr);
    std::string extractTitle(const QByteArray& arr);
    std::string title_;
    std::array<u8, romSize_> rom_;
};
