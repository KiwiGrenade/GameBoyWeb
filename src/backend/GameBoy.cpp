#include "GameBoy.hpp"
#include "InterruptController.hpp"
#include "SerialDataTransfer.hpp"
#include <cstdint>

#include<QElapsedTimer>
#include <mutex>

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

void GameBoy::setRenderer(Renderer *r) {
    const std::lock_guard<std::mutex> lock(mutex_);
    ppu_.setRenderer(r);
}

void GameBoy::loadCartridge(const std::shared_ptr<Cartridge> cartridge) {
    memory_.loadCartridge(cartridge); 
    isRomLoaded_ = true;
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
        ppu_.update(cyclesPassed);
    }
    return cyclesPassed;
}

void GameBoy::stop() {
    isStopped_ = true;
    if(thread_.joinable())
        thread_.join();
}

void GameBoy::pause() {
    isPaused_ = true;
}

void GameBoy::resume() {
    if(not isPaused_)
        return;
    std::lock_guard<std::mutex> lock(mutex_);
    isPaused_ = false;
    pause_cv_.notify_one();
}

void GameBoy::run() {
    isPaused_ = false;
    isStopped_ = false;

    while(not isStopped_) {
        std::unique_lock<std::mutex> lock(mutex_);
        pause_cv_.wait(lock, [this] { return !isPaused_; });
        step(70224);
    }
    isPaused_ = true;
}

void GameBoy::reset() {
    stop();
    ic_.reset();
    timer_.reset();
    joypad_.reset();
    serial_.reset();
    ppu_.reset();
    memory_.reset();
    cpu_.reset();
    romTitle_ = {};
    isRomLoaded_ = false;
}

uint64_t GameBoy::step(uint64_t n) {
    uint64_t cyclesPassed = 0;
    for(size_t i = 0; i < n; ++i) {
        u16 cycles = cpu_.step();
        ppu_.update(cyclesPassed);
        timer_.update(cyclesPassed);
        cyclesPassed += cycles;
    }
    return cyclesPassed;
}


void GameBoy::press(Joypad::Button b) {
    joypad_.press(b);
}


void GameBoy::release(Joypad::Button b) {
    joypad_.release(b);
}

bool GameBoy::isRunning() const {
    std::lock_guard<std::mutex> lock(mutex_);
    return not isPaused_;
}

