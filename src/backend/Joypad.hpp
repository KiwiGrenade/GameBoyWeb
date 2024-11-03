#pragma once

#include "utils.hpp"

#include "Memory.hpp"
#include "CPU.hpp"

class Joypad {
public:
    enum class Button {
        A,
        B,
        Up,
        Down,
        Left,
        Right,
        Start,
        Select
    };

    Joypad(Memory::Joypad& joypad, CPU& cpu);

    void press(const Button);
    void release(const Button);
    void reset();

protected:
    void updateButton(const Button button, const bool pressed);

    Memory::Joypad& joypad_;
    CPU& cpu_;
};

