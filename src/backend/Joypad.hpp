#pragma once

#include "utils.hpp"
class Processor;

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

    Joypad(Processor& ic);

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
    Processor& cpu_;
    u8 buttons_;
};

