#include "Cartridge.hpp"
#include "utils.hpp"

Cartridge::Cartridge(const QByteArray& arr)
    : title_(extractTitle(arr))
    , rom_(extractROM(arr)){
}

std::array<u8, Cartridge::romSize_> Cartridge::extractROM(const QByteArray& arr) {
    if(arr.size() < romSize_)
        Utils::error("Array too small to initialize cartridge ROM");
    std::array<u8, Cartridge::romSize_> rom;
    for(u16 i = 0; i < romSize_; ++i) {
        rom[i] = arr[i];
    }
    return rom;
}

std::string Cartridge::extractTitle(const QByteArray& arr) {
    if(arr.size() < 0x134+15)
        Utils::error("Failed to extract cartridge title: array too small");
    std::string title = "";
    for(u8 i = 0; i < 15; ++i) {
        title += arr[0x134+i];
    }
    return title;
}
