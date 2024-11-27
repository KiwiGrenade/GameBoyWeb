#include "Rom.hpp"
#include "utils.hpp"
#include <cmath>
#include <fstream>
#include <qstringview.h>
#include <vector>

Rom::Rom(std::istream &is) {
    if (not is.good()) {
        Utils::error("ROM: Invalid input stream!");
    }
    Bank buf{};
    while (is.read(reinterpret_cast<char *>(buf.data()), buf.size()))
        data_.push_back(buf);
}

Rom::Rom(const QByteArray arr) {
    if (arr.size() < bankSize_ * 2) {
        Utils::error("ROM: Not enough data!");
    }
    Bank buf{};
    for(u8 i = 0; i < std::ceil(arr.size() / bankSize_); i++) {
        for(u16 j = 0; j < bankSize_; ++j)
            buf[j] = arr[i*bankSize_ + j];
        data_.push_back(buf);
    }
}

Rom::Rom(const std::string &filePathStr) {
    const std::filesystem::path filePath{filePathStr};
    if (filePath.empty())
        Utils::error("Filename not provided for cartridge!");
    if (not std::filesystem::exists(filePath))
        Utils::error("Filename: \"" + filePathStr + " does not exist!");

    Bank buf{};

    std::ifstream file{filePath, std::ios::binary};
    if (file) {
        while(file.read(reinterpret_cast<char *>(buf.data()), buf.size()))
            data_.push_back(buf);
    }
    file.close();
}

u8 Rom::read(u8 bank, u16 addr) const {
    if (bank > data_.size() || addr > data_[0].size())
        Utils::error("Invalid ROM address!");
    return data_[bank][addr];
}
