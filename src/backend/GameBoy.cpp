#include "GameBoy.hpp"
#include "InterruptController.hpp"
#include "SerialDataTransfer.hpp"
#include <cstdint>

#include<QElapsedTimer>

GameBoy::GameBoy()
    : ic_(InterruptController())
    , timer_(Timer(ic_))
    , joypad_(Joypad(ic_))
    , serial_(SerialDataTransfer(ic_))
    , ppu_(PPU(ic_, memory_, cpu_))
    , memory_(Memory(ic_, timer_, joypad_, serial_, ppu_, cpu_))
    , cpu_(CPU(ic_, memory_)) {
}

GameBoy::~GameBoy() {
    stop();
}

void GameBoy::reset() {
    ic_.reset();
    timer_.reset();
    joypad_.reset();
    serial_.reset();
    ppu_.reset();
    memory_.reset();
    cpu_.reset();
}

void GameBoy::stop() {
    isStopped_ = true;
}

void GameBoy::pause() {
    isPaused_ = !isPaused_;
}

void GameBoy::emulateStep() {
    u8 cycles = cpu_.step();
    timer_.update(cycles);
}

std::string GameBoy::getCPUDebugDump() {
    return cpu_.getDebugDump().toStr();
}

std::string GameBoy::getSerialOutput() {
    std::vector<char> vec { serial_.getTestOutput()};
    return std::string {vec.begin(), vec.end()};
}

uint64_t GameBoy::update(const uint32_t cyclesToExecute) {
    uint64_t cyclesPassed = 0;
    while(cyclesPassed < cyclesToExecute) {
        cyclesPassed += cpu_.step();
        // TODO: Check if this is right
        timer_.update(cyclesPassed);
        /*ppu_.update(cyclesPassed);*/
    }
    return cyclesPassed;
}

void GameBoy::run() {
    isPaused_ = false;
    isStopped_ = false;

    QElapsedTimer timer;
    // time between loop iterations
    uint64_t deltaTime = 0;
    // time accumulator
    uint64_t accuTime = 0;
    // 4194304 (DMG-1 clocks/second) / 60 (frames / second)
    constexpr uint32_t frequnecy = 4194304;
    constexpr uint32_t fps = 60;
    constexpr uint32_t nCyclesInWaitTime = frequnecy / fps;
    // 1/60 of second in nanoseconds
    constexpr uint32_t waitTimeNsecs = 16670000;
    constexpr uint64_t maxWaitTimeNsecs = 1000000000;

    timer.start();
    uint64_t cycles = 0;
    while(!isStopped_ && cycles < 1200000) {
        deltaTime = timer.nsecsElapsed();
        timer.restart();

        if(deltaTime > maxWaitTimeNsecs)
            deltaTime = maxWaitTimeNsecs;

        accuTime += deltaTime;
        for(;accuTime >= waitTimeNsecs; accuTime -= waitTimeNsecs) {
            update(nCyclesInWaitTime);
            qDebug() << "Emulator is running!" << Qt::endl;
        }
    }
}
