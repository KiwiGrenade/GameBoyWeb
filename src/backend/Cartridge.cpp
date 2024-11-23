#include "Cartridge.hpp"
#include <array>
#include <filesystem>
#include <fstream>
#include <memory>
#include <qstringview.h>
#include "Ram.hpp"
#include "utils.hpp"

Cartridge::Cartridge(const std::string &filePath)
        : rom_(filePath), title_("") {
}

Cartridge::Cartridge(const QByteArray &fileContent)
        : rom_(fileContent), title_("") {
    initMBC();
}

u8 Cartridge::read(u16 addr) const {
    u8 res;
    if(mbc_) {
        res = mbc_->read(addr);
    }
    else {
        u8 bankNumber = addr / Rom::bankSize_; 
        u16 relAddr = addr - bankNumber * Rom::bankSize_;
        res = rom_.read(bankNumber, relAddr);
    }
    return res;
}

void Cartridge::write(u8 byte, u16 addr) {
    if(mbc_)
        mbc_->write(byte, addr);
}

void Cartridge::initRam() {
    if(hasRam_)
        switch (rom_.read(0, 0x149)) {
            case 0:
                break;
            case 2:
                ram_ = ERam(1);
                break;
            case 3:
                ram_ = ERam(4);
                break;
            case 4:
                ram_ = ERam(16);
                break;
            case 5:
                ram_ = ERam(8);
                break;
        }
}

void Cartridge::initMBC() {
    switch (rom_.read(0, 0x147)) {
        // NO MBC
        case 0x00:
            break;
        // MBC1
        case 0x01:
            mbc_ = std::make_unique<Mbc1>(&rom_, &ram_);
            break;
        // MBC1 + RAM
        case 0x02:
            mbc_ = std::make_unique<Mbc1>(&rom_, &ram_);
            hasRam_ = true;
            break;
        default:
            Utils::error("Unsuported cartridge type!");
    }
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
