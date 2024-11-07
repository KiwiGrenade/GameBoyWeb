#pragma once

#include <memory>

#include <QByteArray>
#include <QThread>

#include "Cartridge.hpp"
#include "InterruptController.hpp"
#include "Memory.hpp"
#include "CPU.hpp"
#include "SerialDataTransfer.hpp"
#include "Timer.hpp"

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
    std::string getSerialOutput();
    void run() override; // start
    uint64_t update(uint32_t nCycles);

private:
    // flags
    bool isPaused;
    bool isStopped;
    InterruptController ic_;
    Timer timer_;
    Joypad joypad_;
    Memory memory_;
    SerialDataTransfer serial_;
    CPU cpu_;
};
