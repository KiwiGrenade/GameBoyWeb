#pragma once

#include <condition_variable>
#include <cstdint>
#include <memory>

#include <QByteArray>
#include <QThread>
#include <thread>

#include "Cartridge.hpp"
#include "Joypad.hpp"
#include "SerialDataTransfer.hpp"
#include "Timer.hpp"
#include "PPU.hpp"
#include "Memory.hpp"
#include "Processor.hpp"

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
    Timer timer_ {cpu_};
    Joypad joypad_ {cpu_};
    SerialDataTransfer serial_ {cpu_};
    Ppu ppu_ {memory_, cpu_};
    Memory memory_ {timer_, joypad_, serial_, ppu_, cpu_};
    Processor cpu_ {[this](uint16_t adr){ return memory_.read(adr); }, // memory read callback
        [this](uint8_t b, uint16_t adr){ memory_.write(b, adr); } // memory write callback 
    };
};
