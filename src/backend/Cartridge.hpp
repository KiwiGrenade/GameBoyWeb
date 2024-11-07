#pragma once

#include "utils.hpp"

class Cartridge {
public:
    Cartridge() = default;
    Cartridge(const std::string& filePath);
    
    inline u8 read(const u16 addr) { return rom_[addr]; };
    inline std::string getTitle() { return title_; };

    static constexpr u16 romSize_ = 0x8000;
private:
    std::array<u8, romSize_> extractROM(const std::string& filePathStr);
    std::string extractTitle();
    std::array<u8, romSize_> rom_;
    std::string title_;
};
