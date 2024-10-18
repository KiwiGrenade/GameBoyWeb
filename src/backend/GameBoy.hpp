#pragma once

#include <memory>

#include <QByteArray>

#include "Cartridge.hpp"
#include "Memory.hpp"
#include "CPU.hpp"

class GameBoy {
public:
    GameBoy();
    ~GameBoy() = default;
    
    void loadCartridge(const std::shared_ptr<Cartridge> cartridge) { memory_->loadCartridge(cartridge); }
    inline const std::shared_ptr<Memory> getMemory() { return memory_; }

private:
    std::shared_ptr<Memory> memory_;
    std::shared_ptr<CPU> cpu_;
};
