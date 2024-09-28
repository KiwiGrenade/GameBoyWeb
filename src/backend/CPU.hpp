#pragma once

#include "utils.hpp"
#include "Instruction.hpp"
#include "RegisterPair.hpp"
#include "Memory.hpp"

#include <QJsonObject>

typedef RegisterPair    r16;
typedef Register        r8;
typedef std::array<Instruction, 256> InstrArray;
typedef std::array<std::function<bool()>, 256> ProcArray;

class CPU {
public:
    CPU(Memory&);
    ~CPU() = default;
    
    void step();

protected:
    Memory&     memory_;
    uint64_t    cycles_; // T-cycles

    // move this somewhere else???
    bool isPrefixed_;

    InstrArray unprefInstrArray_;
    InstrArray prefInstrArray_;
    
    InstrArray getInstrArray(const bool prefixed);
    ProcArray getUnprefProcArray();
    ProcArray getPrefProcArray();
    void handleFlags(const Utils::flagArray& flags);

    u16 SP_; // Stack pointer
    u16 PC_; // Program Counter

    // registers
    r16 AF_; // Accumulator and flags
    r16 BC_;
    r16 DE_;
    r16 HL_;

    r8& A_;
    r8& F_;
    r8& B_;
    r8& C_;
    r8& D_;
    r8& E_;
    r8& H_;
    r8& L_;


    class Flag {
    public:
        Flag(r8& flags, const u8 bitPos)
            : flagsRegister_(flags)
            , bitPos_(bitPos) {
        }

        inline bool val() const { return Utils::getBit(flagsRegister_, bitPos_); }
        inline void set() { Utils::setBit(flagsRegister_, bitPos_); }
        inline void clear() { Utils::clearBit(flagsRegister_, bitPos_); }
        inline void handle(const Utils::Flag flag) {
            switch(flag) {
                case Utils::Flag::set:
                    set();
                    break;
                case Utils::Flag::reset:
                    clear();
                    break;
                default:
                    break;
            }
        }
        inline void complement() {
            if(val())
                clear();
            else
                set();
        }

    private:
        u8 bitPos_;
        r8& flagsRegister_;
    };

    Flag FlagZ_;
    Flag FlagN_;
    Flag FlagH_;
    Flag FlagC_;

// Find a way to move instructions somewhere else
    // instructions
        // miscallaneous
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
