#pragma once

#include "utils.hpp"
#include <vector>

class Rom {
public:
    constexpr static auto bankSize = 0x4000;
    using Bank = std::array<u8, bankSize>;
    
    explicit Rom() = default;
    explicit Rom(std::istream &is);

    u8 read(u8 bank, u16 addr) const;

private:
    std::vector<Bank> data_;
};
