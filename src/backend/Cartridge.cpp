#include "Cartridge.hpp"
#include <array>
#include <cinttypes>
#include <filesystem>
#include <fstream>
#include <qstringview.h>
#include "utils.hpp"

Cartridge::Cartridge(const std::string &filePath)
        : rom_(extractROM(filePath)), title_(extractTitle()) {
}

Cartridge::Cartridge(const QByteArray &fileContent)
        : rom_(extractROM(fileContent)), title_(extractTitle()) {
}

std::array <u8, Cartridge::romSize_> Cartridge::extractROM(const QByteArray &fileContent) {
    if (fileContent.isEmpty())
        Utils::error("File content is empty!");
    if (fileContent.size() < romSize_)
        Utils::error("File content is too small for rom!");

    std::array <u8, Cartridge::romSize_> rom;

    for (uint32_t i = 0; i < romSize_; i++) {
        rom[i] = fileContent[i];
    }

    return rom;
}

std::array <u8, Cartridge::romSize_> Cartridge::extractROM(const std::string &filePathStr) {
    const std::filesystem::path filePath{filePathStr};
    if (filePath.empty())
        Utils::error("Filename not provided for cartridge!");
    if (not std::filesystem::exists(filePath))
        Utils::error("Filename: \"" + filePathStr + " does not exist!");

    std::array <u8, Cartridge::romSize_> rom;

    std::ifstream file{filePath, std::ios::binary};
    if (file) {
        file.read(reinterpret_cast<char *>(rom.data()), static_cast<long>(romSize_));
        if (file.eof())
            Utils::warning("Reached end of file!");
    }
    file.close();

    return rom;
}

std::string Cartridge::extractTitle() {
    auto titleStart = rom_.begin() + 0x0134;
    return std::string(titleStart, titleStart + 15);
}
