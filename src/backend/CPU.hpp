#pragma once

#include "utils.hpp"
#include "Instruction.hpp"
#include "RegisterPair.hpp"
#include "Memory.hpp"

#include <QJsonObject>

typedef RegisterPair    r16;
typedef Register        r8;
typedef std::array<Instruction, 256> InstrArray;
typedef std::array<std::function<void()>, 256> ProcArray;

class CPU {
public:
    CPU(Memory&);
    ~CPU() = default;
    
    void step();

protected:
    Memory&     memory_;
    uint64_t    cycles_; // T-cycles


    // helper flags 
    bool isPrefixed_;
    bool isHalted_;
    bool isHaltBug_;
    bool isEISet_;
    bool isDISet_;
    bool is2xSpeed_;
    bool isStopped_;
    bool isCondMet_;
    bool incrementPC_;

    InstrArray unprefInstrArray_;
    InstrArray prefInstrArray_;
    
    InstrArray getInstrArray(const bool prefixed);
    ProcArray getUnprefProcArray();
    ProcArray getPrefProcArray();
    void handleIME();
    void handleFlags(const Utils::flagArray& flags);
    void reset();

    r16 SP_; // Stack pointer
    r16 PC_; // Program Counter

    // registers
    r16 AF_; // Accumulator and flags
    r16 BC_;
    r16 DE_;
    r16 HL_;

    // half-registers
    r8& A_;
    r8& F_; // flags register
    r8& B_;
    r8& C_;
    r8& D_;
    r8& E_;
    r8& H_;
    r8& L_;
    
    // interrupts
    bool IME_;
    u8& IE_; // memory[0xFFFF]
    u8& IF_; // memory[0xFF0F]

    class Flag {
    public:
        Flag(r8& flags, const u8 bitPos)
            : flagsRegister_(flags)
            , bitPos_(bitPos) {
        }
        inline operator bool() const {
            return Utils::getBit(flagsRegister_, bitPos_);
        }
        inline void set(const bool arg) { Utils::setBit(flagsRegister_, bitPos_, arg); }
        inline void handle(const Utils::Flag flag) {
            switch(flag) {
                case Utils::Flag::set:
                    set(true);
                    break;
                case Utils::Flag::reset:
                    set(false);
                    break;
                default:
                    break;
            }
        }
        inline void complement() { set(!Utils::getBit(flagsRegister_, bitPos_)); }
        static inline bool checkH(const u8 a, const u8 b, const u8 res) {
            return ((a ^ b ^ res) & 0x10);
        }
        static inline bool checkH16(const u16 a, const u16 b, const u16 res) {
            return ((a ^ b ^ res) & 0x1000);
        }

    private:
        u8 bitPos_;
        r8& flagsRegister_;
    };

    Flag FlagZ_;
    Flag FlagN_;
    Flag FlagH_;
    Flag FlagC_;

    inline void checkFlagZ() {
        FlagZ_.set(A_); 
    }

    u8 inline fetch8(const u16 addr) { return memory_.read(addr); };
    u16 fetch16(const u16 addr) {
        u8 lo = fetch8(addr);
        u8 hi = fetch8(addr+1);
        return static_cast<u16>(hi << 8 | lo);
    };
// Find a way to move instructions somewhere else
    // instructions
        // miscallaneous
    void inline ccf();
    void inline cpl();
    void daa();
    void inline di();
    void inline ei();
    void halt();
    void nop();
    void inline scf();
    void inline stop();
        // jump and subroutines
    void jr(const bool cond, int8_t dest);
    void ret(const bool cond);
    void jp(const bool cond, const u16 addr);
    void call(const bool cond, const u16 addr);
    void reti();
    void rst(const u8 n);
        // 8 bit arithmetic and logic
        // 16 bit arithmetic
        // bit operation
        // bit shift
        // load
    void inline ld(r8& to, const r8 from);
    void inline inc(r8&);
    void inline inc(r16&);
    void incd(u16 const addr);
    void inline dec(r8&);
    void inline dec(r16&);
    void decd(u16 const addr);
    void inline ldd(u16 const addr, const u8 byte);
    void inline ld16(r16&, const u16);
    void ldd16(u16 const addr, const r16 word);
    void ldhl_sp(const int8_t d);
    /*void LD(r8, u8);*/
    /*void LD(r16, n16);*/
        // stack operations
    void pop(r16&);
    void popAF();
    void push(const r16 rp);
    void rlc(r8& r);
    void rlca();
    void rrc(r8& r);
    void rrca();
    void rl(r8& r);
    void rla();
    void rr(r8& r);
    void rra();
};
