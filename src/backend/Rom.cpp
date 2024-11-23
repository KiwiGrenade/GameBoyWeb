#include "Rom.hpp"
#include "utils.hpp"
#include <vector>

Rom::Rom(std::istream &is) {
    if (not is.good()) {
        Utils::error("ROM: Invalid input stream!");
    }
    Bank buf{};
    while (is.read(reinterpret_cast<char *>(buf.data()), buf.size()))
        data_.push_back(buf);
}

u8 Rom::read(u8 bank, u16 addr) const {
    if (bank > data_.size() || addr > data_[0].size())
        Utils::error("Invalid ROM address!");
    return data_[bank][addr];
}
