#include "Joypad.hpp"
#include "Processor.hpp"
#include "utils.hpp"

Joypad::Joypad(Processor& cpu)
    : cpu_(cpu)
    , inputDevice_(InputDevice::Nothing)
    , buttons_(0) {
}

void Joypad::press(const Button b) {
    Utils::setBit(buttons_, b, true);
}

void Joypad::release(const Button b) {
    Utils::setBit(buttons_, b, false);
}

void Joypad::reset() {
    inputDevice_ = InputDevice::Nothing;
    buttons_ = 0;
}

void Joypad::write(u8 byte) {
    bool buttons = Utils::getBit(byte, 5);
    bool pad = Utils::getBit(byte, 4);
    // buttons are selected if 5-th bit is 0
    if(not buttons && pad) {
        inputDevice_ = InputDevice::Buttons;
    }
    else if(buttons && not pad) {
        // selected pad
        inputDevice_ = InputDevice::Pad;
    }
    else if(buttons && pad) {
        inputDevice_ = InputDevice::Nothing;
    }
    else {
        Utils::error("Both input devices selected!");
    }
}

u8 Joypad::read() {
    u8 hi = 0;
    u8 lo = 0;
    u8 res = 0;
    switch (inputDevice_) {
        case InputDevice::Buttons:
            hi = 0b11010000;
            lo = buttons_ & 0x0F;
            res = hi | lo;
            break;
        case InputDevice::Pad:
            hi = 0b11100000;
            lo = (buttons_ & 0xF0) >> 4;
            res = hi | lo;
            break;
        case InputDevice::Nothing:
            res = 0xFF;
            break;
    }
    return res;
}
