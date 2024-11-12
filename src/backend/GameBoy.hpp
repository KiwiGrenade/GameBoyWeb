#pragma once

#include <condition_variable>
#include <cstdint>
#include <memory>

#include <QByteArray>
#include <QThread>
#include <thread>

#include "Cartridge.hpp"
#include "InterruptController.hpp"
#include "Joypad.hpp"
#include "SerialDataTransfer.hpp"
#include "Timer.hpp"
#include "PPU.hpp"
#include "Memory.hpp"
#include "CPU.hpp"

class GameBoy {
public:
    GameBoy();
    ~GameBoy();
    
    void runConcurrently();
    void setRenderer(Renderer *r);
    void execute(uint32_t cycles);

    void stop();
    void pause();
    void resume();
    void reset();
    uint64_t step(uint64_t cycles);

    void press(Joypad::Button button);
    void release(Joypad::Button button);
    bool isRunning() const;

    void loadCartridge(const std::shared_ptr<Cartridge> cartridge);
    std::string getCPUDebugDump();
    std::string getSerialOutput();
    uint64_t update(uint32_t nCycles);

protected:
    void run();
    std::string romTitle_ {};
    bool isRomLoaded_ = false;

    std::thread thread_;
    
    std::condition_variable pause_cv_;
    std::atomic<bool> isStopped_ = false;
    mutable std::mutex mutex_;
    // flags
    bool isPaused_ = false;
    InterruptController ic_;
    Timer timer_;
    Joypad joypad_;
    SerialDataTransfer serial_;
    PPU ppu_;
    Memory memory_;
    CPU cpu_;
};
