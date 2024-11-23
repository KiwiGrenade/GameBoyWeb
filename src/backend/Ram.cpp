#include "Ram.hpp"
#include "utils.hpp"
#include <vector>

template<u16 bankSize>
Ram<bankSize>::Ram(u8 banks, const std::vector <u8> &sram)
        : data_(banks) {
    if (not sram.empty())
        load(sram);
}

template<u16 bankSize>
u8 Ram<bankSize>::read(u8 bank, u16 addr) const {
    return data_[bank][addr];
}

template<u16 bankSize>
void Ram<bankSize>::write(u8 byte, u8 bank, u16 addr) {
    data_[bank][addr] = byte;
}

template<u16 bankSize>
void Ram<bankSize>::load(const std::vector <u8> &sram) {
    size_t i = 0;
    for (Bank &bank: data_) {
        std::copy(sram.begin() + i, sram.begin() + i + bankSize, bank.data());
        i += bankSize;
    }
}

template<u16 bankSize>
void Ram<bankSize>::resize(u8 nBanks) {
    data_.resize(nBanks);
}

template<u16 bankSize>
uint64_t Ram<bankSize>::getSize() const {
    return bankSize * data_.size();
}

template<u16 bankSize>
void Ram<bankSize>::reset() {
    for (u8 i = 0; i < data_.size(); ++i)
        data_[i] = {};
}

template
class Ram<0x2000>;

template
class Ram<0x1000>;
