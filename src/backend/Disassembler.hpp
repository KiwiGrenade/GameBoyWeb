#pragma once

#include "utils.hpp"
#include <cstdint>
#include <vector>

class Disassembler {
public:
    Disassembler();
    ~Disassembler() = default;

    /*std::vector<Assembly> disassemble();*/

private:
    
    Assembly disassembleOneOpcode(uint8_t* byte);
    bool prefix_;
};


