#pragma once

#include <vector>
#include <array>

#include "utils.hpp"

template<u16 bankSize>
class Ram {
public:
    static const u16 bankSize_ = bankSize;
    using Bank = std::array<u8, bankSize>;

    explicit Ram(u8 banks = 0, const std::vector <u8> &load = {});

    u8 read(u8 bank, u16 addr) const;
    void write(u8 byte, u8 bank, u16 addr);
    void load(const std::vector <u8> &load);
    void resize(u8 nBanks);
    uint64_t getSize() const;
    void reset();

private:
    std::vector <Bank> data_{};

};

extern template
class Ram<0x2000>;

extern template
class Ram<0x1000>;

using WorkRam = Ram<0x1000>;
using VRam = Ram<0x2000>;
using ERam = Ram<0x2000>;
