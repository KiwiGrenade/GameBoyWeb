#pragma once

#include <array>
#include "utils.hpp"

#include <QByteArray>

class Memory {
public:
    Memory();
    ~Memory() = default;
    
    inline u8 read(u16 idx) const { return memory_[idx]; }
    inline void write(const u8 byte, const u16 idx) { memory_[idx] = byte; }
    inline u8& getIE() { return memory_[0xFFFF]; }
    inline u8& getIF() { return memory_[0xFF0F]; }
    void loadSimpleROM(const QByteArray& rom);
    

    static constexpr uint32_t size_ = 0x10000;
private:
    std::array<u8, size_> memory_;
};
