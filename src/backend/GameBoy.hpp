#pragma once

#include <memory>

#include <QByteArray>
#include <QThread>

#include "Cartridge.hpp"
#include "Memory.hpp"
#include "CPU.hpp"

class GameBoy : public QThread {
    Q_OBJECT
public:
    GameBoy();
    ~GameBoy();
    
    void loadCartridge(const std::shared_ptr<Cartridge> cartridge) { memory_->loadCartridge(cartridge); }
    void reset();
    void stop();
    void pause();
    void run() override; // start
    uint64_t update(uint32_t nCycles);
    inline const std::shared_ptr<Memory> getMemory() { return memory_; }

private:
    // flags
    bool isPaused;
    bool isStopped;
    std::shared_ptr<Memory> memory_;
    std::shared_ptr<CPU> cpu_;
};
