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
    void disableInterrupt(u8);
    void setIE(u8 byte);
    u8 getIE();
    void setIF(u8 byte);
    u8 getIF();
    void reset();

private:
    // Address: 0xFFFF
    u8 IE_;
    // Address: 0xFF0F
    u8 IF_;
};
