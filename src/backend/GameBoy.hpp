#pragma once

#include <memory>

#include <QByteArray>

#include "Memory.hpp"
#include "CPU.hpp"

class GameBoy {
public:
    GameBoy();
    ~GameBoy() = default;
    
    inline void loadROM(const QByteArray& rom) { memory_->loadSimpleROM(rom); }
    inline const std::shared_ptr<Memory> getMemory() { return memory_; }

private:
    std::shared_ptr<Memory> memory_;
    std::shared_ptr<CPU> cpu_;
};
