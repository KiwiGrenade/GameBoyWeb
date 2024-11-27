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

    Joypad(InterruptController &ic);

    void press(const Button);
    void release(const Button);
    void reset();
    u8 read();
    void write(u8 byte);

private:
    void update(const Button b, const bool isPressed);

    InterruptController &ic_;
    u8 selectButtons_;
    u8 selectPad_;
    u8 buttons_;
    u8 directions_;
};

