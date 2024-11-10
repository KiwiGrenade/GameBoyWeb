#pragma once

#include "utils.hpp"
#include "InterruptController.hpp"

class PPU {
    PPU(InterruptController& ic);

protected:
    InterruptController& ic_;
};
