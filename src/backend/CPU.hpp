#pragma once

#include "utils.hpp"
#include "RegisterPair.hpp"

class CPU {
public:
    CPU();
    ~CPU() = default;
    
    void step();

/*private:*/
    RegisterPair AF_; // Accumulator and flags
    RegisterPair BC_;
    RegisterPair DE_;
    RegisterPair HL_;

    Register& A_;
    Register& F_;
    Register& B_;
    Register& C_;
    Register& D_;
    Register& E_;
    Register& H_;
    Register& L_;

    u16 SP_; // Stack pointer
    u16 PC_; // Program Counter
};
