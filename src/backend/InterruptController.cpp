#include "InterruptController.hpp"
#include "utils.hpp"

void InterruptController::requestInterrupt(Type intType) {
    Utils::setBit(interrupts_, intType, true);
}

void InterruptController::write(u8 byte) {
    interrupts_ = byte;
}

u8 InterruptController::read() {
    return interrupts_;
}

void InterruptController::reset() {
    interrupts_ = 0;
}
