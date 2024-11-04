#pragma once

#include "utils.hpp"

class InterruptController {
public:
    enum Type {
        VBlank,
        LCD,
        Timer,
        Serial,
        Joypad
    };

    InterruptController() = default;

    void requestInterrupt(Type);
    void write(u8 byte);
    u8 read();
    void reset();

protected:
    u8 interrupts_;
};
