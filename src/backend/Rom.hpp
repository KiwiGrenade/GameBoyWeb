#pragma once

#include "utils.hpp"
#include <QByteArray>
#include <vector>

class Rom {
public:
    constexpr static auto bankSize_ = 0x4000;
    using Bank = std::array<u8, bankSize_>;

    explicit Rom() = default;
    explicit Rom(const QByteArray);
    explicit Rom(const std::string& filePathStr);
    explicit Rom(std::istream &is);

    u8 read(u8 bank, u16 addr) const;

private:
    std::vector <Bank> data_;
};
