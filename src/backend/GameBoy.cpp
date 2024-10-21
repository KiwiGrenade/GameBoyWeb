#include "GameBoy.hpp"
#include <cstdint>
#include <memory>

#include<QElapsedTimer>

GameBoy::GameBoy()
    : memory_(std::make_shared<Memory>())
    , cpu_(std::make_shared<CPU>(*memory_)){
}

GameBoy::~GameBoy() {
    stop();
}

void GameBoy::reset() {
    memory_->reset();
    cpu_->reset();
}

void GameBoy::stop() {
    isStopped = true;
}

void GameBoy::pause() {
    isPaused = !isPaused;
}

uint64_t GameBoy::update(const uint32_t cyclesToExecute) {
    uint64_t cyclesPassed = 0;
    while(cyclesPassed < cyclesToExecute) {
        cyclesPassed += cpu_->step();
    }
    return cyclesPassed;
}

void GameBoy::run() {
    isPaused = false;
    isStopped = false;

    QElapsedTimer timer;
    uint64_t deltaTime = 0;
    uint64_t accuTime = 0;

    constexpr uint32_t maxCycles = 69905; // 4194304 (DMG-1 clocks/second) / 60 (frames / second)
    timer.start();
    while(!isStopped) {
        deltaTime = timer.nsecsElapsed();
        timer.restart();

        if(deltaTime > 1000000000)
            deltaTime = 1000000000;

        accuTime += deltaTime;
        for(;accuTime >= 16670000; accuTime -= 16670000) {
            update(maxCycles);
            qDebug() << "Emulator is running!" << Qt::endl;
        }
    }
}
