#pragma once

#include <memory>

#include <QByteArray>

#include "Memory.hpp"

class GameBoy {
    public:
        GameBoy();
        ~GameBoy() = default;
        
        inline void loadROM(const QByteArray& rom) { memory_->load(rom); }

    private:
        std::shared_ptr<Memory> memory_;
};
