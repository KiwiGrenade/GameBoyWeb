#pragma once

#include <cstdint>
#include <functional>

#include "utils.hpp"
#include "InstructionInfo.hpp"
#include "InterruptController.hpp"
#include "RegisterPair.hpp"
#include "DebugTypes.hpp"
#include "Memory.hpp"

typedef RegisterPair    r16;
typedef Register        r8;

class CPU {
public:
    CPU(InterruptController& ic, Memory& memory);

    void step();
    void reset();
    CPUDump getDebugDump() const noexcept;
    uint32_t getCycles() const noexcept { return cycles_; }
    // Manually add cycles to cycle count. This is useful for OAM DMA transfers: they need to take
    // 160 machine cycles (640 clock cycles).
    void addCycles(uint32_t c);
    bool isStopped() const noexcept { return isStopped_; }
    bool isHalted() const noexcept { return isHalted_; }

    private:

    InterruptController& ic_;
    Memory& memory_;

    enum Flags : u8
    {
        CARRY = 1 << 4,
        HALF = 1 << 5,
        NEGATIVE = 1 << 6,
        ZERO = 1 << 7,
    };

    r16 af_, bc_, de_, hl_, sp_, pc_;
    uint32_t cycles_ {0};
    bool isStopped_{false};
    bool isHalted_{false};
    bool isBranchCycle_ {false};
    bool IME_ {false};
    bool isEiSet_ {false};
    bool isDiSet_ {false}; // for tracking when EI and DI are
        // called, since they don't take effect until the next instruction
    bool isHaltBug_{false};

    u8 fetch8();
    u16 fetch16();

    // flags
    bool getFlag(Flags f) const;
    void setFlag(Flags, bool);

    bool executeInterrupt(u8 i);
    bool handleInterrupts();

    // instructions
    // misc/control
    void nop() const {}
    void stop() { isStopped_ = true; }
    void halt();
    void di();
    void ei();
    // jump and calls
    void jr(bool, const int8_t);
    void ret(bool);
    void jp(bool, const u16);
    void call(bool, const u16);
    void rst(const u8);
    void reti(); // ?
    // 8-bit load
    void ld(u8 &a, u8 b) { a = b; }
    void ldd(u16 adr, u8 b) { memory_.write(b, adr); }
    void ldd_sp(u16 adr);
    void ldr(const r16 &rp, u8 b) { memory_.write(b, rp); }

    // 16-bit load
    void ld(r16 &a, const u16 b) { a = b; }
    void ldhl_sp(const int8_t);
    void push(const u16);
    void pop(r16 &);
    void pop_af(); // special logic for AF needed
    // 8-bit arithmetic and logic
    void inc(u8 &);
    void inc_i(u16 adr); // we could refactor this indirect addressing
                              // using decorators with templates (todo...)
    void dec(u8 &);
    void dec_i(u16 adr);
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
    // 8-bit rotations and shifts
    void rlc(u8 &);
    void rlca();
    void rlc_i(u16 adr);
    void rrc(u8 &);
    void rrca();
    void rrc_i(u16 adr);
    void rl(u8 &);
    void rla();
    void rl_i(u16 adr);
    void rr(u8 &);
    void rra();
    void rr_i(u16 adr);
    void sla(u8 &);
    void sla_i(u16 adr);
    void sra(u8 &);
    void sra_i(u16 adr);
    void swap(u8 &);
    void swap_i(u16 adr);
    void srl(u8 &);
    void srl_i(u16 adr);
    void bit(const u8, const u8);
    void res(const u8, u8 &);
    void res_i(u8, u16 adr);
    void set(const u8, u8 &);
    void set_i(u8, u16 adr);

    static constexpr u16 IO_MEMORY {0xff00};
};
