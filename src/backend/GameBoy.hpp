#pragma once

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

    // main loop
    void runConcurrently();

    // state managment
    uint32_t step();
    void stop();
    void pause();
    void reset();

    // setters
    void setRenderer(Renderer *r);
    void loadCartridge(const std::shared_ptr <Cartridge> cartridge);
    void press(Joypad::Button button);
    void release(Joypad::Button button);

    // getters
    std::string getCPUDebugDump();
    std::string getSerialOutput();

protected:
    void run();
    void executeNCycles(uint64_t cycles);

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
