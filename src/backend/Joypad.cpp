#include "Joypad.hpp"
#include "utils.hpp"

Joypad::Joypad(Memory::Joypad& joypad, CPU& cpu)
    : joypad_(joypad)
    , cpu_(cpu) {
}

void Joypad::press(const Button b) {
    updateButton(b, true);
}

void Joypad::release(const Button b) {
    updateButton(b, false);
}

void Joypad::reset() {
    joypad_.reset();
}

void Joypad::updateButton(const Button button, const bool state) {
    switch(button) {
        case Button::A:
            Utils::setBit(joypad_.buttons_, 0, state);
            break;
        case Button::B:
            Utils::setBit(joypad_.buttons_, 1, state);
            break;
        case Button::Select:
            Utils::setBit(joypad_.buttons_, 2, state);
            break;
        case Button::Start:
            Utils::setBit(joypad_.buttons_, 3, state);
            break;
        case Button::Right:
            Utils::setBit(joypad_.buttons_, 4, state);
            break;
        case Button::Left:
            Utils::setBit(joypad_.buttons_, 5, state);
            break;
        case Button::Up:
            Utils::setBit(joypad_.buttons_, 6, state);
            break;
        case Button::Down:
            Utils::setBit(joypad_.buttons_, 7, state);
            break;
    }
}
