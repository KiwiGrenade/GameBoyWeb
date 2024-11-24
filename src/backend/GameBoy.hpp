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
    void executeNCycles(uint64_t cycles);
    uint32_t step();
    void stop();
    void pause();
    void resume();
    void reset();
    void press(Joypad::Button button);
    void release(Joypad::Button button);
    bool isRunning() const;
    void loadCartridge(const std::shared_ptr <Cartridge> cartridge);
    std::string getCPUDebugDump();
    std::string getSerialOutput();
    uint64_t update(uint32_t nCycles);

protected:
    void run();

    std::string romTitle_{};
    bool isRomLoaded_ = false;

    std::thread thread_;

    std::atomic<bool> isStopped_ = false;
    mutable std::mutex mutex_;
    // flags
    bool isPaused_ = false;
    InterruptController ic_{};
    CPUClock cpuClock_{};
    Timer timer_{ic_};
    Joypad joypad_{ic_};
    SerialDataTransfer serial_{ic_};
    PPU ppu_{ic_, cpuClock_};
    Memory memory_{ic_, timer_, joypad_, serial_, ppu_, cpuClock_};
    CPU cpu_{ic_, cpuClock_, memory_};
};
