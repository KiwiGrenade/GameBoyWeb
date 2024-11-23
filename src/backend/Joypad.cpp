#include "Joypad.hpp"
#include "utils.hpp"

Joypad::Joypad(InterruptController &ic)
        : ic_(ic), selectButtons_(false), selectPad_(false), buttons_(0) {
}

void Joypad::press(const Button b) {
    update(b, true);
    /*Utils::setBit(buttons_, b, true);*/
}

void Joypad::release(const Button b) {
    update(b, false);
}

void Joypad::update(const Button b, bool isPressed) {
    switch (b) {
        case Button::A:
            Utils::setBit(buttons_, 0, isPressed);
            break;
        case Button::B:
            Utils::setBit(buttons_, 1, isPressed);
            break;
        case Button::Select:
            Utils::setBit(buttons_, 2, isPressed);
            break;
        case Button::Start:
            Utils::setBit(buttons_, 3, isPressed);
            break;
        case Button::Right:
            Utils::setBit(directions_, 0, isPressed);
            break;
        case Button::Left:
            Utils::setBit(directions_, 1, isPressed);
            break;
        case Button::Up:
            Utils::setBit(directions_, 2, isPressed);
            break;
        case Button::Down:
            Utils::setBit(directions_, 3, isPressed);
            break;
    }
}

void Joypad::reset() {
    selectButtons_ = false;
    selectPad_ = false;
    buttons_ = 0;
}

void Joypad::write(u8 byte) {
    selectButtons_ = Utils::getBit(byte, 4);
    selectPad_ = Utils::getBit(byte, 5);
}

u8 Joypad::read() {
    u8 res = 0xC0;

    Utils::setBit(res, 5, selectButtons_);
    Utils::setBit(res, 4, selectPad_);

    if (selectButtons_)
        res |= buttons_;
    if (selectPad_)
        res |= directions_;

    return ~res;
}
