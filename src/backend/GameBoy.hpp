#pragma once

#include <memory>

#include <QByteArray>
#include <QThread>

#include "Cartridge.hpp"
#include "InterruptController.hpp"
#include "Joypad.hpp"
#include "SerialDataTransfer.hpp"
#include "Timer.hpp"
#include "PPU.hpp"
#include "Memory.hpp"
#include "CPU.hpp"

class GameBoy : public QThread {
    Q_OBJECT
public:
    GameBoy();
    ~GameBoy();
    
    void loadCartridge(const std::shared_ptr<Cartridge> cartridge) { memory_.loadCartridge(cartridge); }
    void reset();
    void stop();
    void pause();
    void emulateStep();
    std::string getCPUDebugDump();
    std::string getSerialOutput();
    void run() override; // start
    uint64_t update(uint32_t nCycles);

protected:
    // flags
    bool isPaused_ = false;
    bool isStopped_ = false;
    InterruptController ic_;
    Timer timer_;
    Joypad joypad_;
    SerialDataTransfer serial_;
    PPU ppu_;
    Memory memory_;
    CPU cpu_;
};
