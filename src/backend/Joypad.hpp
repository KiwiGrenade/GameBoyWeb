#pragma once

#include "utils.hpp"
#include "InterruptController.hpp"

class Joypad {
public:
    enum Button {
        A,
        B,
        Select,
        Start,
        Right,
        Left,
        Up,
        Down
    };

    Joypad(InterruptController& ic);

    void press(const Button);
    void release(const Button);
    void reset();
    u8 read();
    void write(u8 byte);

protected:
    enum class InputDevice {
        Buttons,
        Pad,
        Nothing
    };

    InputDevice inputDevice_;
    InterruptController& ic_;
    u8 buttons_;
};

