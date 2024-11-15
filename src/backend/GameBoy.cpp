#include "GameBoy.hpp"
#include "InterruptController.hpp"
#include "SerialDataTransfer.hpp"
#include "utils.hpp"
#include <cstdint>

#include<QElapsedTimer>
#include <mutex>

GameBoy::GameBoy() {}

void GameBoy::setRenderer(Renderer *r) {
    /*const std::lock_guard<std::mutex> lock(mutex_);*/
    ppu_.set_renderer(r);
}

void GameBoy::loadCartridge(const std::shared_ptr<Cartridge> cartridge) {
    memory_.loadCartridge(cartridge); 
    isRomLoaded_ = true;
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
        size_t oldCycles = cpu_.getCycles();
        cpu_.step();
        cyclesPassed += (cpu_.getCycles() - oldCycles);
        ppu_.step(cyclesPassed);
        timer_.update(cyclesPassed);
    }
    return cyclesPassed;
}

void GameBoy::pause() {
    isPaused_ = true;
}

void GameBoy::resume() {
    if(not isPaused_)
        return;
    /*std::lock_guard<std::mutex> lock(mutex_);*/
    isPaused_ = false;
    /*pause_cv_.notify_one();*/
}

void GameBoy::run() {
    if(not isRomLoaded_) {
        Utils::error("Rom is not loaded!");
    }
    isPaused_ = false;
    isStopped_ = false;
    
    while(not isStopped_) {
        /*std::unique_lock<std::mutex> lock(mutex_);*/
        /*pause_cv_.wait(lock, [this] { return !isPaused_; });*/
        executeNCycles(70224);
    }
    isPaused_ = true;
}

void GameBoy::reset() {
    /*stop();*/
    timer_.reset();
    joypad_.reset();
    serial_.reset();
    ppu_.reset();
    memory_.reset();
    cpu_.reset();
    romTitle_ = {};
    isRomLoaded_ = false;
}

void GameBoy::executeNCycles(uint64_t cycles) {
    uint64_t cyclesPassed = 0;
    while(cyclesPassed < cycles) {
        cyclesPassed += step();
    }
}

uint32_t GameBoy::step() {
    uint32_t cyclesPassed = 0;
    size_t oldCycles {cpu_.getCycles()};
    cpu_.step();
    cyclesPassed += (cpu_.getCycles() - oldCycles);
    ppu_.step(cyclesPassed);
    timer_.update(cyclesPassed);
    return cyclesPassed;
}

void GameBoy::press(Joypad::Button b) {
    joypad_.press(b);
}


void GameBoy::release(Joypad::Button b) {
    joypad_.release(b);
}

bool GameBoy::isRunning() const {
    return not isPaused_;
}

