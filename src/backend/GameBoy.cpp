#include "GameBoy.hpp"
#include "utils.hpp"
#include <cstdint>

#include<QElapsedTimer>
#include <mutex>

GameBoy::GameBoy() {}

GameBoy::~GameBoy() {
    stop();
}

void GameBoy::setRenderer(Renderer *r) {
    const std::lock_guard <std::mutex> lock(mutex_);
    ppu_.setRenderer(r);
}

void GameBoy::loadCartridge(const std::shared_ptr <Cartridge> cartridge) {
    memory_.loadCartridge(cartridge);
    isRomLoaded_ = true;
}

std::string GameBoy::getCPUDebugDump() {
    return cpu_.getDebugDump().toStr();
}

std::string GameBoy::getSerialOutput() {
    std::vector<char> vec{serial_.getTestOutput()};
    return std::string{vec.begin(), vec.end()};
}

void GameBoy::stop() {
    isStopped_ = true;
    if (thread_.joinable())
        thread_.join();
}

void GameBoy::pause() {
    const std::lock_guard <std::mutex> lock(mutex_);
    isPaused_ = !isPaused_;
}

void GameBoy::run() {
    isPaused_ = false;
    isStopped_ = false;

    QElapsedTimer timer;
    uint64_t deltaTime = 0;
    uint64_t accuTime = 0;

    constexpr uint32_t interval = 16750418;
    constexpr uint32_t maxInterval = 1000000000;

    timer.start();
    while(not isStopped_) {
        const std::lock_guard <std::mutex> lock(mutex_);
        deltaTime = timer.nsecsElapsed();
        timer.restart();

        if(deltaTime > maxInterval)
            deltaTime = maxInterval;

        accuTime += deltaTime;
        for(;accuTime >= interval; accuTime -= interval) {
            executeNCycles(70224);
        }
    }
}

void GameBoy::reset() {
    stop();
    timer_.reset();
    joypad_.reset();
    serial_.reset();
    ppu_.reset();
    memory_.reset();
    cpu_.reset();
    isRomLoaded_ = false;
}

void GameBoy::executeNCycles(uint64_t cycles) {
    if (isPaused_)
        return;
    uint64_t cyclesPassed = 0;
    while (cyclesPassed < cycles) {
        cyclesPassed += step();
    }
}

uint32_t GameBoy::step() {
    if (not isRomLoaded_) {
        Utils::error("Rom is not loaded!");
    }
    uint32_t cyclesPassed = 0;
    cpuClock_.cycles_ = 0;
    cpu_.step();
    ppu_.update(cpuClock_.cycles_);
    timer_.update(cpuClock_.cycles_);
    return cpuClock_.cycles_;
}

void GameBoy::runConcurrently() {
    stop();
    auto runFunction = [this] { this->run(); };
    thread_ = std::thread(runFunction);
}

void GameBoy::press(Joypad::Button b) {
    joypad_.press(b);
}


void GameBoy::release(Joypad::Button b) {
    joypad_.release(b);
}

