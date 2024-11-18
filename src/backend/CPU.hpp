#pragma once

#include <cstdint>
#include <functional>

#include "InterruptController.hpp"
#include "Memory.hpp"
#include "RegisterPair.hpp"
#include "DebugTypes.hpp"
#include "CPUClock.hpp"

class CPU
{
    public:

    enum Interrupt : u8
    {
        VBLANK = 0,
        LCD_STAT,
        TIMER,
        SERIAL,
        JOYPAD,
    };

    CPU(InterruptController& ic, CPUClock& clock, Memory& memory_);
    void step();
    void reset();
    CPUDump getDebugDump() const noexcept;
    void request_interrupt(Interrupt i);
    void toggle_double_speed();

    u16 af() const noexcept { return af_; }
    u16 bc() const noexcept { return bc_; }
    u16 de() const noexcept { return de_; }
    u16 hl() const noexcept { return hl_; }
    u16 sp() const noexcept { return sp_; }
    u16 pc() const noexcept { return pc_; }
    uint32_t getCycles() const noexcept { return clock_.cycles_; }
    // Manually add cycles to cycle count. This is useful for OAM DMA transfers: they need to take
    // 160 machine cycles (640 clock cycles).
    void addCycles(uint32_t c);
    bool stopped() const noexcept { return clock_.isStopped_; }
    bool halted() const noexcept { return clock_.isHalted_; }
    bool double_speed() const noexcept { return double_speed_; }

    std::vector<u8> next_ops(u16 n) const;

    private:

    InterruptController& ic_;
    CPUClock& clock_;
    Memory& memory_;

    enum Flags : u8
    {
        CARRY = 1 << 4,
        HALF = 1 << 5,
        NEGATIVE = 1 << 6,
        ZERO = 1 << 7,
    };

    RegisterPair af_, bc_, de_, hl_, sp_, pc_;
    /*uint32_t cycles_ {0};*/
    bool use_branch_cycles_ {false};
    bool ime_ {false};
    bool ei_set_ {false}, di_set_ {false}; // for tracking when EI and DI are
        // called, since they don't take effect until the next instruction
    bool halt_bug_ {false};
    bool double_speed_ {false}; // CGB only

    std::function<u8(u16)> read {[this](u16 adr) {return memory_.read(adr);}};
    std::function<void(u8, u16)> write {[this](u8 b, u16 a) { memory_.write(b, a);}};
    u8 fetch8();
    u16 fetch16();

    // flags
    bool get_flag(Flags f) const;
    void set_flag(Flags, bool);

    bool execute_interrupt(Interrupt i);
    bool check_interrupt();

    // instructions
    // misc/control
    void nop() const {}
    void stop() { clock_.isStopped_ = true; }
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
    void ldd(u16 adr, u8 b) { write(b, adr); }
    void ldd_sp(u16 adr);
    void ldr(const RegisterPair &rp, u8 b) { write(b, rp); }

    // 16-bit load
    void ld(RegisterPair &a, const u16 b) { a = b; }
    void ldhl_sp(const int8_t);
    void push(const u16);
    void pop(RegisterPair &);
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
    void inc(RegisterPair &rp) { ++rp; }
    void dec(RegisterPair &rp) { --rp; }
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
