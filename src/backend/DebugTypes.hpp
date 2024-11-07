#pragma once

#include "utils.hpp"
#include <iomanip>
#include <ios>
#include <sstream>

struct CPUDump {
    std::array<u8, 4> nextOpcodes_;
    u8 A_, F_, B_, C_, D_, E_, H_, L_;
    u16 SP_, PC_;
    bool IME_;

    std::string toStr() {
        std::stringstream ss;
        ss << std::hex << std::setfill('0') << std::right
            << "A:" << std::setw(2) << static_cast<int>(A_)
            << " F:" << std::setw(2) << static_cast<int>(F_)
            << " B:" << std::setw(2) << static_cast<int>(B_)
            << " C:" << std::setw(2) << static_cast<int>(C_)
            << " D:" << std::setw(2) << static_cast<int>(D_)
            << " E:" << std::setw(2) << static_cast<int>(E_)
            << " H:" << std::setw(2) << static_cast<int>(H_)
            << " L:" << std::setw(2) << static_cast<int>(L_)
            << " SP:" << std::setw(4) << static_cast<int>(SP_)
            << " PC:" << std::setw(4) << static_cast<int>(PC_)
            << " PCMEM:" << std::setw(2) << static_cast<int>(nextOpcodes_[0])
            << "," << std::setw(2) << static_cast<int>(nextOpcodes_[1])
            << "," << std::setw(2) << static_cast<int>(nextOpcodes_[2])
            << "," << std::setw(2) << static_cast<int>(nextOpcodes_[3]);
        return ss.str();
    }
};
