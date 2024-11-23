#pragma once

#include "utils.hpp"
#include "Rom.hpp"
#include "Ram.hpp"
#include "MemoryBankController.hpp"

#include <QByteArray>
#include <optional>

class Cartridge {
public:
    Cartridge() = default;
    Cartridge(const std::string &filePath);
    Cartridge(const QByteArray &fileContent);

    u8 read(const u16 addr) const;
    void write(const u8 byte, const u16 addr);
    inline std::string getTitle() { return title_; };
    static constexpr u16
    romSize_ = 0x8000;

private:
    void initMBC();
    void initRam();
    std::array <u8, romSize_> extractROM(const std::string &filePathStr);
    std::array <u8, romSize_> extractROM(const QByteArray &fileContent);
    std::string extractTitle();

    Rom rom_;
    bool hasRam_ {false};
    std::optional<ERam> ram_ {std::nullopt};
    std::unique_ptr<MemoryBankController> mbc_ {nullptr};



    std::string title_;
};
