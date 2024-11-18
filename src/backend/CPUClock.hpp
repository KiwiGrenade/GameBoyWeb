#pragma once
#include <stdint.h>
struct CPUClock {
    void reset() {
        cycles_ = 0;
        isStopped_ = false;
        isHalted_ = false;
    }
    uint32_t cycles_ = 0;
    bool isStopped_ = false;
    bool isHalted_ = false;
};
