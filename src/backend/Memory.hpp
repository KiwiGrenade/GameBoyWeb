#pragma once

#include <memory>
#include <array>
#include "utils.hpp"

#include <QByteArray>

class Memory {
public:
    Memory();
    ~Memory() = default;
    
    inline u8 read(u16 idx) const { return memory_->at(idx); }
    inline void write(const u8 byte, const u16 idx) { memory_->at(idx) = byte; }
    void loadSimpleROM(const QByteArray& rom);

    static constexpr u16 size_ = 0xFFFF;
private:
    std::shared_ptr<std::array<u8, size_>> memory_;
};
