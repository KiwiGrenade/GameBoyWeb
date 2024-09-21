#pragma once

#include "utils.hpp"
#include "RegisterPair.hpp"
#include "Memory.hpp"
#include <functional>
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

    struct InstrInfo {
        InstrInfo(const QJsonObject& obj)
        : bytes_(getBytesFromJsonObject(obj))
        , cycles_(getCyclesFromJson(obj))
        , flags_(getFlagsFromJsonObject(obj)) {
        }

        u8 bytes_;
        std::pair<u8, u8> cycles_;
        std::array<Utils::Flag, 4> flags_;

        static u8 getBytesFromJsonObject(const QJsonObject& obj);
        static std::pair<u8, u8> getCyclesFromJson(const QJsonObject& obj);
        static std::array<Utils::Flag, 4> getFlagsFromJsonObject(const QJsonObject& obj);
    };

    std::unordered_map<u8, InstrInfo> unprefInstrMap_;
    std::unordered_map<u8, InstrInfo> prefInstrMap_;

    uint64_t    cycles_; // T-cycles
    bool        isPrefixed_;

    // memory access
    /*u8 fetchByte();*/
    /*u16 fetchWord();*/

    // returns true if opcode condition was met
    [[nodiscard]] bool executeOpcode(u8);
    

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
    void handleFlags();
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
