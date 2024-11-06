#include "Cartridge.hpp"
#include <array>
#include <filesystem>
#include <fstream>
#include "utils.hpp"

Cartridge::Cartridge(const std::string& filePath) 
    : rom_(extractROM(filePath))
    , title_(extractTitle()) {
}

std::array<u8, Cartridge::romSize_> Cartridge::extractROM(const std::string& filePathStr) {
    const std::filesystem::path filePath { filePathStr };
    if(not std::filesystem::exists(filePath))
        Utils::error("Filename not provided for cartridge!");

    std::array<u8, Cartridge::romSize_> rom;
    
    std::ifstream file { filePath, std::ios::binary };
    if(file) {
        file.read(reinterpret_cast<char*>(rom.data()), static_cast<long>(romSize_));
        if(file.eof())
            Utils::warning("Reached end of file!");
    }
    file.close();

    return rom;
}

std::string Cartridge::extractTitle() {
    auto titleStart = rom_.begin() + 0x0134;
    return std::string(titleStart, titleStart + 15);
}
