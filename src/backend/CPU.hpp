#pragma once

#include "Instruction.hpp"
#include "utils.hpp"
#include "RegisterPair.hpp"
#include "Memory.hpp"
#include <qjsonobject.h>
#include <unordered_map>

typedef RegisterPair    r16;
typedef Register&        r8;

class CPU {
public:
    CPU(Memory&);
    ~CPU() = default;
    
    void step();

private:
    Memory& memory_;
    uint64_t    cycles_; // T-cycles
    // instruction booleans - move them somewhere???
    bool isPrefixed_;
    bool isCondMet_;

    std::unordered_map<u8, Instruction::Info> unprefInstrInfoMap_;
    std::unordered_map<u8, Instruction::Info> prefInstrInfoMap_;

    // memory access
    /*u8 fetchByte();*/
    /*u16 fetchWord();*/

    // returns true if opcode condition was met
    void executeOpcode(u8);
    inline bool getFlagZ() {
        return Utils::getBit(flags_, 7);
    }
    inline bool getFlagN() {
        return Utils::getBit(flags_, 6);
    }
    inline bool getFlagH() {
        return Utils::getBit(flags_, 5);
    }
    inline bool getFlagC() {
        return Utils::getBit(flags_, 4);
    }
    

    r16 AF_; // Accumulator and flags
    r16 BC_;
    r16 DE_;
    r16 HL_;

    r8 A_;
    r8 B_;
    r8 C_;
    r8 D_;
    r8 E_;
    r8 H_;
    r8 L_;

    r8 flags_;

    u16 sp_; // Stack pointer
    u16 pc_; // Program Counter
    
    // instructions
        // info
        // miscallaneous
    // set or reset flags
    void handleFlags(const Utils::flagArray& flags);
    void ccf();
    void cpl();
    void daa();
    void di();
    void ei();
    void halt();
    void nop();
    void scf();
    void stop();
        // jump and subroutines
        // 8 bit arithmetic and logic
        // 16 bit arithmetic
        // bit operation
        // bit shift
        // load
        // stack operations

};
