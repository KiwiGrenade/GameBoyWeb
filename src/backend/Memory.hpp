#pragma once

#include <memory>
#include <cstdint>
#include <array>

class Memory {
public:
    Memory();
    ~Memory() = default;
    
    inline uint8_t read(uint16_t idx) { return memory_->at(idx); }
    inline void write(uint8_t byte, uint16_t idx) { memory_->at(idx) = byte; }
    void loadFromFile();

private:
    static constexpr uint16_t size_ = 0xFFFF;
    std::shared_ptr<std::array<uint8_t, size_>> memory_;
};
