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

    void notImplemented() { Utils::error("Opcode not implemented!"); }

    /*################### 8-bit arithmetic and logic ###################*/

    void inline inc(r8&);
    void incd(u16 const addr);
    void inline dec(r8&);
    void decd(u16 const addr);
    void addHelper(const u8, const bool);
    void add(const u8);
    void adc(const u8); // add with carry flag
    void subHelper(const u8, const bool);
    void sub(const u8);
    void sbc(const u8); // sub with carry flag
    void andr(const u8);
    void xorr(const u8);
    void orr(const u8);
    void cp(const u8);

    /*################### 16-bit arithmetic and logic ###################*/

    void add16(const u16);
    void inline inc(r16&);
    void inline dec(r16&);

    /*################### bit operations ###################*/

    // TODO: Add tests
    void bit(const u8 n, const r8 r);
    // TODO: Add tests
    void bitHL(const u8 n);
    // TODO: Add tests
    void res(const u8 n, r8& r);
    // TODO: Add tests
    void resHL(const u8 n);
    // TODO: Add tests
    void set(const u8 n, r8& r);
    // TODO: Add tests
    void setHL(const u8 n);
    // TODO: Add tests
    void swap(r8& r);
    // TODO: Add tests
    void swapHL();

    /*################### bit shift ###################*/

    void rl(r8& r);
    void rla();
    void rlca();
    void rr(r8& r);
    void rra();
    void rrca();
    void rlHL();
    void rlc(r8& r);
    void rlcHL();
    void rrHL();
    void rrc(r8& r);
    void rrcHL();
    // TODO: Add tests
    void sla(r8& r);
    // TODO: Add tests
    void slaHL();
    // TODO: Add tests
    void sra(r8& r);
    // TODO: Add tests
    void sraHL();
    // TODO: Add tests
    void srl(r8& r);
    // TODO: Add tests
    void srlHL();

    /*################### LOAD ###################*/

    void inline ld(r8& to, const r8 from);
    void inline ld16(r16&, const u16);
    void ldd(u16 const addr, const u8 byte);
    void ldd16(u16 const addr, const r16 word);

    /*################### JUMP and SUBROUTINES ###################*/

    void jp(const bool cond, const u16 addr);
    void jr(const bool cond, int8_t dest);
    void call(const bool cond, const u16 addr);
    void rst(const u8 n);
    void ret(const bool cond);
    void reti();

    /*################### STACK ###################*/

    void pop(r16&);
    void popAF();
    void push(const r16 rp);
    void ldHLSP(const int8_t d);
    void addSP();

    /*################### misc ###################*/

    void inline nop();
    void inline scf();
    void inline ccf();
    void inline cpl();
    void inline ei();
    void inline di();
    void inline stop();
    void halt();
    void daa();

};
