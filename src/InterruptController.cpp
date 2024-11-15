#include "InterruptController.hpp"
#include "utils.hpp"

void InterruptController::requestInterrupt(Type intType) {
    Utils::setBit(IF_, intType, true);
}

void InterruptController::disableInterrupt(u8 i) {
    Utils::setBit(IF_, i, false);
}

void InterruptController::setIE(u8 byte) {
    IE_ = byte;
}

void InterruptController::setIF(u8 byte) {
    IF_ = byte;
}

u8 InterruptController::getIE() {
    return IE_;
}

u8 InterruptController::getIF() {
    return IF_;
}

void InterruptController::reset() {
    IF_ = 0;
    IE_ = 0;
}
