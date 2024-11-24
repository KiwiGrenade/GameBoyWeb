#pragma once

#include <cstdint>
#include <functional>

#include "InterruptController.hpp"
#include "Memory.hpp"
#include "RegisterPair.hpp"
#include "DebugTypes.hpp"
#include "CPUClock.hpp"

typedef RegisterPair r16;
typedef u8& r8;

class CPU {
public:

    CPU(InterruptController &ic, CPUClock &clock, Memory &memory_);

    void step();
    void reset();
    CPUDump getDebugDump() const noexcept;
    
    // needed for OAM DMA transfer
    void addCycles(uint32_t c);
    bool getIsStopped() const noexcept { return clock_.isStopped_; }
    bool getIsHalted() const noexcept { return clock_.isHalted_; }
    std::vector <u8> nextOps(u16 n) const;

private:

    InterruptController &ic_;
    CPUClock &clock_;
    Memory &memory_;

    enum Flags : u8 {
        CARRY = 1 << 4,
        HALF = 1 << 5,
        NEGATIVE = 1 << 6,
        ZERO = 1 << 7,
    };

    r16 AF_, BC_, DE_, HL_, SP_, PC_;
    r8 A_, F_, B_, C_, D_, E_, H_, L_;

    // IME_ isn't set immediately
    bool IME_{false};
    bool eiSet_{false};
    bool diSet_{false};

    bool useBranchCycles_{false};
    bool doubleSpeed_{false}; // CGB only

    std::function<u8(u16)> read{[this](u16 adr) { return memory_.read(adr); }};
    std::function<void(u8, u16)> write{[this](u8 b, u16 a) { memory_.write(b, a); }};

    u8 fetch8();
    u16 fetch16();

    // flags
    bool getFlag(Flags f) const;
    void setFlag(Flags, bool);

    // interrupts
    bool executeInterrupt(InterruptController::Type i);
    bool checkInterrupt();

    // instructions
    // misc/control
    void nop() const {}
    void stop() { clock_.isStopped_ = true; }
    void halt();
    void di();
    void ei();

    // 8-bit load
    void ld(u8 &a, u8 b) { a = b; }
    void ldd(u16 adr, u8 b) { write(b, adr); }
    void lddSp(u16 adr);
    void ldr(const r16 &rp, u8 b) { write(b, rp); }

    // 16-bit load
    void ld(r16 &a, const u16 b) { a = b; }
    void ldhlSp(const int8_t);
    void push(const u16);
    void pop(r16 &);
    void popAf(); // special logic for AF needed

    // jump and calls
    void jr(bool, const int8_t);
    void ret(bool);
    void jp(bool, const u16);
    void call(bool, const u16);
    void rst(const u8);
    void reti();
    // 8-bit arithmetic and logic
    void inc(u8 &);
    void incInd(u16 adr);
    void dec(u8 &);
    void decInd(u16 adr);
    void daa();
    void scf();
    void cpl();
    void ccf();
    void adc(const u8, bool);
    void sbc(const u8, bool);
    void andr(const u8);
    void xorr(const u8);
    void orr(const u8);
    void cp(const u8);

    // 16-bit arithmetic and logic
    void inc(r16 &rp) { ++rp; }
    void dec(r16 &rp) { --rp; }
    void add(const u16);
    void add_sp(int8_t);

    // 8-bit manipulation
    void rrc(u8 &);
    void rrca();
    void rrcInd(u16 adr);
    void rlc(u8 &);
    void rlca();
    void rlcInd(u16 adr);
    void rr(u8 &);
    void rra();
    void rrInd(u16 adr);
    void rl(u8 &);
    void rla();
    void rlInd(u16 adr);
    void sra(u8 &);
    void sraInd(u16 adr);
    void sla(u8 &);
    void slaInd(u16 adr);
    void swap(u8 &);
    void swapInd(u16 adr);
    void srl(u8 &);
    void srlInd(u16 adr);
    void bit(const u8, const u8);
    void res(const u8, u8 &);
    void resInd(u8, u16 adr);
    void set(const u8, u8 &);
    void setInd(u8, u16 adr);

    static constexpr u16
    IO_MEMORY { 0xff00 };
};
