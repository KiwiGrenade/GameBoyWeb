#include <iomanip>
#include <chrono>
#include <ctime>

#include "CPU.hpp"
#include "InterruptController.hpp"
#include "InstructionInfo.hpp"

CPU::CPU(InterruptController& ic, CPUClock& clock, Memory& memory)
    : ic_(ic)
    , clock_(clock)
    , memory_(memory)
    , A_(AF_.hi_)
    , F_(AF_.lo_)
    , B_(BC_.hi_)
    , C_(BC_.lo_)
    , D_(DE_.hi_)
    , E_(DE_.lo_)
    , H_(HL_.hi_)
    , L_(HL_.lo_)
{
    reset();
}

void CPU::reset()
{
    AF_ = 0x01b0;
    BC_ = 0x0013;
    DE_ = 0x00d8;
    HL_ = 0x014d;
    SP_ = 0xfffe;
    PC_ = 0x0100;
    clock_.reset();
    useBranchCycles_ = false;
    IME_ = false;
    eiSet_ = false;
    diSet_ = false;
    doubleSpeed_ = false;
}

void CPU::addCycles(uint32_t c)
{
    clock_.cycles_ += c;
}

bool CPU::getFlag(Flags f) const
{
    return (F_ & f);
}

void CPU::setFlag(Flags f, bool cond)
{
    u8 mask;
    if (cond)
        mask = 0xff;
    else
        mask = 0;
    F_ = (F_ & ~f) | (mask & f);
}

u8 CPU::fetch8()
{
    u8 op = read(PC_);
    // HALT bug: the processor fails to increment the PC_
    ++PC_;
    return op;

}

uint16_t CPU::fetch16()
{
    u8 lo = fetch8();
    u8 hi = fetch8();
    return static_cast<uint16_t>(hi << 8 | lo);
}

bool CPU::executeInterrupt(Interrupt i)
{
    clock_.isHalted_ = false;
    clock_.isStopped_ = false;
    if (!IME_)
        return false; // don't service the interrupt if IME is disable

    write(PC_.hi_, --SP_);
    write(PC_.lo_, --SP_);

    PC_.hi_ = 0;
    PC_.lo_ = 0x40 + i*8;

    IME_ = false;
    ic_.disableInterrupt(i);
    return true;
}

bool CPU::checkInterrupt()
{
    u8 int_enable {read(0xffff)};
    u8 int_flag {read(0xff0f)};
    u8 request = int_enable & int_flag;
    bool serviced {false};

    if (request & 1)
        serviced = executeInterrupt(VBLANK);
    else if (request & 1 << 1)
        serviced = executeInterrupt(LCD_STAT);
    else if (request & 1 << 2)
        serviced = executeInterrupt(TIMER);
    else if (request & 1 << 3)
        serviced = executeInterrupt(SERIAL);
    else if (request & 1 << 4)
        serviced = executeInterrupt(JOYPAD);
    return serviced;
}

void CPU::step()
{
    if (checkInterrupt())
        return;
    if (eiSet_) // EI was called last cycle
    {
        eiSet_ = false;
        IME_ = true;
    }
    else if (diSet_) // DI was called last cycle
    {
        diSet_ = false;
        IME_ = false;
    }
    if (clock_.isHalted_)
    {
        clock_.cycles_ += 4; // assume NOP when CPU is halted
        return;
    }
    u8 op {fetch8()};
    switch (op)
    {
        // misc and control
        case 0x00: nop(); break;
        case 0x10: stop(); break;
        case 0x76: halt(); break;
        case 0xf3: di(); break;
        case 0xfb: ei(); break;

        // jump and calls
        case 0x18: jr(true, static_cast<int8_t>(fetch8())); break;
        case 0x20: jr(!getFlag(ZERO), static_cast<int8_t>(fetch8())); break;
        case 0x28: jr(getFlag(ZERO), static_cast<int8_t>(fetch8())); break;
        case 0x30: jr(!getFlag(CARRY), static_cast<int8_t>(fetch8())); break;
        case 0x38: jr(getFlag(CARRY), static_cast<int8_t>(fetch8())); break;

        case 0xc0: ret(!getFlag(ZERO)); break;
        case 0xc8: ret(getFlag(ZERO)); break;
        case 0xc9: ret(true); break;
        case 0xd0: ret(!getFlag(CARRY)); break;
        case 0xd8: ret(getFlag(CARRY)); break;

        case 0xc2: jp(!getFlag(ZERO), fetch16()); break;
        case 0xc3: jp(true, fetch16()); break;
        case 0xca: jp(getFlag(ZERO), fetch16()); break;
        case 0xd2: jp(!getFlag(CARRY), fetch16()); break;
        case 0xda: jp(getFlag(CARRY), fetch16()); break;

        case 0xc4: call(!getFlag(ZERO), fetch16()); break;
        case 0xcc: call(getFlag(ZERO), fetch16()); break;
        case 0xcd: call(true, fetch16()); break;
        case 0xd4: call(!getFlag(CARRY), fetch16()); break;
        case 0xdc: call(getFlag(CARRY), fetch16()); break;

        case 0xe9: jp(true, HL_); break;

        case 0xc7: rst(0x00); break;
        case 0xcf: rst(0x08); break;
        case 0xd7: rst(0x10); break;
        case 0xdf: rst(0x18); break;
        case 0xe7: rst(0x20); break;
        case 0xef: rst(0x28); break;
        case 0xf7: rst(0x30); break;
        case 0xff: rst(0x38); break;

        case 0xd9: reti(); break;

        // 8-bit load
        case 0x02: ldr(BC_, A_); break;
        case 0x12: ldr(DE_, A_); break;
        case 0x22: ldr(HL_++, A_); break;
        case 0x32: ldr(HL_--, A_); break;

        case 0x06: ld(B_, fetch8()); break;
        case 0x0e: ld(C_, fetch8()); break;
        case 0x16: ld(D_, fetch8()); break;
        case 0x1e: ld(E_, fetch8()); break;
        case 0x26: ld(H_, fetch8()); break;
        case 0x2e: ld(L_, fetch8()); break;
        case 0x36: ldr(HL_, fetch8()); break;
        case 0x3e: ld(A_, fetch8()); break;

        case 0x0a: ld(A_, read(BC_)); break;
        case 0x1a: ld(A_, read(DE_)); break;
        case 0x2a: ld(A_, read(HL_++)); break;
        case 0x3a: ld(A_, read(HL_--)); break;

        case 0x40: break;
        case 0x41: ld(B_, C_); break;
        case 0x42: ld(B_, D_); break;
        case 0x43: ld(B_, E_); break;
        case 0x44: ld(B_, H_); break;
        case 0x45: ld(B_, L_); break;
        case 0x46: ld(B_, read(HL_)); break;
        case 0x47: ld(B_, A_); break;

        case 0x48: ld(C_, B_); break;
        case 0x49: break;
        case 0x4a: ld(C_, D_); break;
        case 0x4b: ld(C_, E_); break;
        case 0x4c: ld(C_, H_); break;
        case 0x4d: ld(C_, L_); break;
        case 0x4e: ld(C_, read(HL_)); break;
        case 0x4f: ld(C_, A_); break;

        case 0x50: ld(D_, B_); break;
        case 0x51: ld(D_, C_); break;
        case 0x52: break;
        case 0x53: ld(D_, E_); break;
        case 0x54: ld(D_, H_); break;
        case 0x55: ld(D_, L_); break;
        case 0x56: ld(D_, read(HL_)); break;
        case 0x57: ld(D_, A_); break;

        case 0x58: ld(E_, B_); break;
        case 0x59: ld(E_, C_); break;
        case 0x5a: ld(E_, D_); break;
        case 0x5b: break;
        case 0x5c: ld(E_, H_); break;
        case 0x5d: ld(E_, L_); break;
        case 0x5e: ld(E_, read(HL_)); break;
        case 0x5f: ld(E_, A_); break;

        case 0x60: ld(H_, B_); break;
        case 0x61: ld(H_, C_); break;
        case 0x62: ld(H_, D_); break;
        case 0x63: ld(H_, E_); break;
        case 0x64: break;
        case 0x65: ld(H_, L_); break;
        case 0x66: ld(H_, read(HL_)); break;
        case 0x67: ld(H_, A_); break;

        case 0x68: ld(L_, B_); break;
        case 0x69: ld(L_, C_); break;
        case 0x6a: ld(L_, D_); break;
        case 0x6b: ld(L_, E_); break;
        case 0x6c: ld(L_, H_); break;
        case 0x6d: break;
        case 0x6e: ld(L_, read(HL_)); break;
        case 0x6f: ld(L_, A_); break;

        case 0x70: ldr(HL_, B_); break;
        case 0x71: ldr(HL_, C_); break;
        case 0x72: ldr(HL_, D_); break;
        case 0x73: ldr(HL_, E_); break;
        case 0x74: ldr(HL_, H_); break;
        case 0x75: ldr(HL_, L_); break;
        case 0x77: ldr(HL_, A_); break;

        case 0x78: ld(A_, B_); break;
        case 0x79: ld(A_, C_); break;
        case 0x7a: ld(A_, D_); break;
        case 0x7b: ld(A_, E_); break;
        case 0x7c: ld(A_, H_); break;
        case 0x7d: ld(A_, L_); break;
        case 0x7e: ld(A_, read(HL_)); break;
        case 0x7f: break;

        case 0xe0: ldd(IO_MEMORY + fetch8(), A_); break; // ldh (a8),A_
        case 0xea: ldd(fetch16(), A_); break; // ld (a16),A_
        case 0xf0: ld(A_, read(IO_MEMORY + fetch8())); break; // ldh A_,(a8)
        case 0xfa: ld(A_, read(fetch16())); break; // ldh A_,(a16)
        case 0xe2: ldd(IO_MEMORY + C_, A_); break; // ld (C_),A_
        case 0xf2: ld(A_, read(IO_MEMORY + C_)); break; // ld A_,(C_)

        // 16-bit load
        case 0x01: ld(BC_, fetch16()); break;
        case 0x11: ld(DE_, fetch16()); break;
        case 0x21: ld(HL_, fetch16()); break;
        case 0x31: ld(SP_, fetch16()); break;
        case 0x08: lddSp(fetch16()); break;

        case 0xc1: pop(BC_); break;
        case 0xd1: pop(DE_); break;
        case 0xe1: pop(HL_); break;
        case 0xf1: popAf(); break;

        case 0xc5: push(BC_); break;
        case 0xd5: push(DE_); break;
        case 0xe5: push(HL_); break;
        case 0xf5: push(AF_); break;

        case 0xf8: ldhlSp(fetch8()); break;
        case 0xf9: ld(SP_, HL_); break;

        // 8-bit arithmetic
        case 0x27: daa(); break;
        case 0x37: scf(); break;
        case 0x2f: cpl(); break;
        case 0x3f: ccf(); break;

        case 0x04: inc(B_); break;
        case 0x0c: inc(C_); break;
        case 0x14: inc(D_); break;
        case 0x1c: inc(E_); break;
        case 0x24: inc(H_); break;
        case 0x2c: inc(L_); break;
        case 0x34: incInd(HL_); break;
        case 0x3c: inc(A_); break;

        case 0x05: dec(B_); break;
        case 0x0d: dec(C_); break;
        case 0x15: dec(D_); break;
        case 0x1d: dec(E_); break;
        case 0x25: dec(H_); break;
        case 0x2d: dec(L_); break;
        case 0x35: decInd(HL_); break;
        case 0x3d: dec(A_); break;

        case 0x80: adc(B_, false); break;
        case 0x81: adc(C_, false); break;
        case 0x82: adc(D_, false); break;
        case 0x83: adc(E_, false); break;
        case 0x84: adc(H_, false); break;
        case 0x85: adc(L_, false); break;
        case 0x86: adc(read(HL_), false); break;
        case 0x87: adc(A_, false); break;

        case 0x88: adc(B_, getFlag(CARRY)); break;
        case 0x89: adc(C_, getFlag(CARRY)); break;
        case 0x8a: adc(D_, getFlag(CARRY)); break;
        case 0x8b: adc(E_, getFlag(CARRY)); break;
        case 0x8c: adc(H_, getFlag(CARRY)); break;
        case 0x8d: adc(L_, getFlag(CARRY)); break;
        case 0x8e: adc(read(HL_), getFlag(CARRY)); break;
        case 0x8f: adc(A_, getFlag(CARRY)); break;

        case 0x90: sbc(B_, false); break;
        case 0x91: sbc(C_, false); break;
        case 0x92: sbc(D_, false); break;
        case 0x93: sbc(E_, false); break;
        case 0x94: sbc(H_, false); break;
        case 0x95: sbc(L_, false); break;
        case 0x96: sbc(read(HL_), false); break;
        case 0x97: sbc(A_, false); break;

        case 0x98: sbc(B_, getFlag(CARRY)); break;
        case 0x99: sbc(C_, getFlag(CARRY)); break;
        case 0x9a: sbc(D_, getFlag(CARRY)); break;
        case 0x9b: sbc(E_, getFlag(CARRY)); break;
        case 0x9c: sbc(H_, getFlag(CARRY)); break;
        case 0x9d: sbc(L_, getFlag(CARRY)); break;
        case 0x9e: sbc(read(HL_), getFlag(CARRY)); break;
        case 0x9f: sbc(A_, getFlag(CARRY)); break;

        case 0xa0: andr(B_); break;
        case 0xa1: andr(C_); break;
        case 0xa2: andr(D_); break;
        case 0xa3: andr(E_); break;
        case 0xa4: andr(H_); break;
        case 0xa5: andr(L_); break;
        case 0xa6: andr(read(HL_)); break;
        case 0xa7: andr(A_); break;

        case 0xa8: xorr(B_); break;
        case 0xa9: xorr(C_); break;
        case 0xaa: xorr(D_); break;
        case 0xab: xorr(E_); break;
        case 0xac: xorr(H_); break;
        case 0xad: xorr(L_); break;
        case 0xae: xorr(read(HL_)); break;
        case 0xaf: xorr(A_); break;

        case 0xb0: orr(B_); break;
        case 0xb1: orr(C_); break;
        case 0xb2: orr(D_); break;
        case 0xb3: orr(E_); break;
        case 0xb4: orr(H_); break;
        case 0xb5: orr(L_); break;
        case 0xb6: orr(read(HL_)); break;
        case 0xb7: orr(A_); break;

        case 0xb8: cp(B_); break;
        case 0xb9: cp(C_); break;
        case 0xba: cp(D_); break;
        case 0xbb: cp(E_); break;
        case 0xbc: cp(H_); break;
        case 0xbd: cp(L_); break;
        case 0xbe: cp(read(HL_)); break;
        case 0xbf: cp(A_); break;

        case 0xc6: adc(fetch8(), false); break;
        case 0xce: adc(fetch8(), getFlag(CARRY)); break;
        case 0xd6: sbc(fetch8(), false); break;
        case 0xde: sbc(fetch8(), getFlag(CARRY)); break;
        case 0xe6: andr(fetch8()); break;
        case 0xee: xorr(fetch8()); break;
        case 0xf6: orr(fetch8()); break;
        case 0xfe: cp(fetch8()); break;

        // 16-bit arithmetic
        case 0x03: inc(BC_); break;
        case 0x13: inc(DE_); break;
        case 0x23: inc(HL_); break;
        case 0x33: inc(SP_); break;

        case 0x09: add(BC_); break;
        case 0x19: add(DE_); break;
        case 0x29: add(HL_); break;
        case 0x39: add(SP_); break;

        case 0x0b: dec(BC_); break;
        case 0x1b: dec(DE_); break;
        case 0x2b: dec(HL_); break;
        case 0x3b: dec(SP_); break;

        case 0xe8: add_sp(static_cast<int8_t>(fetch8())); break;

        // bit operations
        case 0x07: rlca(); break;
        case 0x0f: rrca(); break;
        case 0x17: rla(); break;
        case 0x1f: rra(); break;

        // prefix cb
        case 0xcb:
        {

            switch (fetch8())
            {
                case 0x00: rlc(B_); break;
                case 0x01: rlc(C_); break;
                case 0x02: rlc(D_); break;
                case 0x03: rlc(E_); break;
                case 0x04: rlc(H_); break;
                case 0x05: rlc(L_); break;
                case 0x06: rlcInd(HL_); break;
                case 0x07: rlc(A_); break;

                case 0x08: rrc(B_); break;
                case 0x09: rrc(C_); break;
                case 0x0a: rrc(D_); break;
                case 0x0b: rrc(E_); break;
                case 0x0c: rrc(H_); break;
                case 0x0d: rrc(L_); break;
                case 0x0e: rrcInd(HL_); break;
                case 0x0f: rrc(A_); break;

                case 0x10: rl(B_); break;
                case 0x11: rl(C_); break;
                case 0x12: rl(D_); break;
                case 0x13: rl(E_); break;
                case 0x14: rl(H_); break;
                case 0x15: rl(L_); break;
                case 0x16: rlInd(HL_); break;
                case 0x17: rl(A_); break;

                case 0x18: rr(B_); break;
                case 0x19: rr(C_); break;
                case 0x1a: rr(D_); break;
                case 0x1b: rr(E_); break;
                case 0x1c: rr(H_); break;
                case 0x1d: rr(L_); break;
                case 0x1e: rrInd(HL_); break;
                case 0x1f: rr(A_); break;

                case 0x20: sla(B_); break;
                case 0x21: sla(C_); break;
                case 0x22: sla(D_); break;
                case 0x23: sla(E_); break;
                case 0x24: sla(H_); break;
                case 0x25: sla(L_); break;
                case 0x26: slaInd(HL_); break;
                case 0x27: sla(A_); break;

                case 0x28: sra(B_); break;
                case 0x29: sra(C_); break;
                case 0x2a: sra(D_); break;
                case 0x2b: sra(E_); break;
                case 0x2c: sra(H_); break;
                case 0x2d: sra(L_); break;
                case 0x2e: sraInd(HL_); break;
                case 0x2f: sra(A_); break;

                case 0x30: swap(B_); break;
                case 0x31: swap(C_); break;
                case 0x32: swap(D_); break;
                case 0x33: swap(E_); break;
                case 0x34: swap(H_); break;
                case 0x35: swap(L_); break;
                case 0x36: swapInd(HL_); break;
                case 0x37: swap(A_); break;

                case 0x38: srl(B_); break;
                case 0x39: srl(C_); break;
                case 0x3a: srl(D_); break;
                case 0x3b: srl(E_); break;
                case 0x3c: srl(H_); break;
                case 0x3d: srl(L_); break;
                case 0x3e: srlInd(HL_); break;
                case 0x3f: srl(A_); break;

                // BIT x,r
                // 00xx x000
                // xxx = bit to change
                case 0x40: bit(0, B_); break;
                case 0x41: bit(0, C_); break;
                case 0x42: bit(0, D_); break;
                case 0x43: bit(0, E_); break;
                case 0x44: bit(0, H_); break;
                case 0x45: bit(0, L_); break;
                case 0x46: bit(0, read(HL_)); break;
                case 0x47: bit(0, A_); break;

                case 0x48: bit(1, B_); break;
                case 0x49: bit(1, C_); break;
                case 0x4a: bit(1, D_); break;
                case 0x4b: bit(1, E_); break;
                case 0x4c: bit(1, H_); break;
                case 0x4d: bit(1, L_); break;
                case 0x4e: bit(1, read(HL_)); break;
                case 0x4f: bit(1, A_); break;

                case 0x50: bit(2, B_); break;
                case 0x51: bit(2, C_); break;
                case 0x52: bit(2, D_); break;
                case 0x53: bit(2, E_); break;
                case 0x54: bit(2, H_); break;
                case 0x55: bit(2, L_); break;
                case 0x56: bit(2, read(HL_)); break;
                case 0x57: bit(2, A_); break;

                case 0x58: bit(3, B_); break;
                case 0x59: bit(3, C_); break;
                case 0x5a: bit(3, D_); break;
                case 0x5b: bit(3, E_); break;
                case 0x5c: bit(3, H_); break;
                case 0x5d: bit(3, L_); break;
                case 0x5e: bit(3, read(HL_)); break;
                case 0x5f: bit(3, A_); break;

                case 0x60: bit(4, B_); break;
                case 0x61: bit(4, C_); break;
                case 0x62: bit(4, D_); break;
                case 0x63: bit(4, E_); break;
                case 0x64: bit(4, H_); break;
                case 0x65: bit(4, L_); break;
                case 0x66: bit(4, read(HL_)); break;
                case 0x67: bit(4, A_); break;

                case 0x68: bit(5, B_); break;
                case 0x69: bit(5, C_); break;
                case 0x6a: bit(5, D_); break;
                case 0x6b: bit(5, E_); break;
                case 0x6c: bit(5, H_); break;
                case 0x6d: bit(5, L_); break;
                case 0x6e: bit(5, read(HL_)); break;
                case 0x6f: bit(5, A_); break;

                case 0x70: bit(6, B_); break;
                case 0x71: bit(6, C_); break;
                case 0x72: bit(6, D_); break;
                case 0x73: bit(6, E_); break;
                case 0x74: bit(6, H_); break;
                case 0x75: bit(6, L_); break;
                case 0x76: bit(6, read(HL_)); break;
                case 0x77: bit(6, A_); break;

                case 0x78: bit(7, B_); break;
                case 0x79: bit(7, C_); break;
                case 0x7a: bit(7, D_); break;
                case 0x7b: bit(7, E_); break;
                case 0x7c: bit(7, H_); break;
                case 0x7d: bit(7, L_); break;
                case 0x7e: bit(7, read(HL_)); break;
                case 0x7f: bit(7, A_); break;

                case 0x80: res(0, B_); break;
                case 0x81: res(0, C_); break;
                case 0x82: res(0, D_); break;
                case 0x83: res(0, E_); break;
                case 0x84: res(0, H_); break;
                case 0x85: res(0, L_); break;
                case 0x86: resInd(0, HL_); break;
                case 0x87: res(0, A_); break;

                case 0x88: res(1, B_); break;
                case 0x89: res(1, C_); break;
                case 0x8a: res(1, D_); break;
                case 0x8b: res(1, E_); break;
                case 0x8c: res(1, H_); break;
                case 0x8d: res(1, L_); break;
                case 0x8e: resInd(1, HL_); break;
                case 0x8f: res(1, A_); break;

                case 0x90: res(2, B_); break;
                case 0x91: res(2, C_); break;
                case 0x92: res(2, D_); break;
                case 0x93: res(2, E_); break;
                case 0x94: res(2, H_); break;
                case 0x95: res(2, L_); break;
                case 0x96: resInd(2, HL_); break;
                case 0x97: res(2, A_); break;

                case 0x98: res(3, B_); break;
                case 0x99: res(3, C_); break;
                case 0x9a: res(3, D_); break;
                case 0x9b: res(3, E_); break;
                case 0x9c: res(3, H_); break;
                case 0x9d: res(3, L_); break;
                case 0x9e: resInd(3, HL_); break;
                case 0x9f: res(3, A_); break;

                case 0xa0: res(4, B_); break;
                case 0xa1: res(4, C_); break;
                case 0xa2: res(4, D_); break;
                case 0xa3: res(4, E_); break;
                case 0xa4: res(4, H_); break;
                case 0xa5: res(4, L_); break;
                case 0xa6: resInd(4, HL_); break;
                case 0xa7: res(4, A_); break;

                case 0xa8: res(5, B_); break;
                case 0xa9: res(5, C_); break;
                case 0xaa: res(5, D_); break;
                case 0xab: res(5, E_); break;
                case 0xac: res(5, H_); break;
                case 0xad: res(5, L_); break;
                case 0xae: resInd(5, HL_); break;
                case 0xaf: res(5, A_); break;

                case 0xb0: res(6, B_); break;
                case 0xb1: res(6, C_); break;
                case 0xb2: res(6, D_); break;
                case 0xb3: res(6, E_); break;
                case 0xb4: res(6, H_); break;
                case 0xb5: res(6, L_); break;
                case 0xb6: resInd(6, HL_); break;
                case 0xb7: res(6, A_); break;

                case 0xb8: res(7, B_); break;
                case 0xb9: res(7, C_); break;
                case 0xba: res(7, D_); break;
                case 0xbb: res(7, E_); break;
                case 0xbc: res(7, H_); break;
                case 0xbd: res(7, L_); break;
                case 0xbe: resInd(7, HL_); break;
                case 0xbf: res(7, A_); break;

                case 0xc0: set(0, B_); break;
                case 0xc1: set(0, C_); break;
                case 0xc2: set(0, D_); break;
                case 0xc3: set(0, E_); break;
                case 0xc4: set(0, H_); break;
                case 0xc5: set(0, L_); break;
                case 0xc6: setInd(0, HL_); break;
                case 0xc7: set(0, A_); break;

                case 0xc8: set(1, B_); break;
                case 0xc9: set(1, C_); break;
                case 0xca: set(1, D_); break;
                case 0xcb: set(1, E_); break;
                case 0xcc: set(1, H_); break;
                case 0xcd: set(1, L_); break;
                case 0xce: setInd(1, HL_); break;
                case 0xcf: set(1, A_); break;

                case 0xd0: set(2, B_); break;
                case 0xd1: set(2, C_); break;
                case 0xd2: set(2, D_); break;
                case 0xd3: set(2, E_); break;
                case 0xd4: set(2, H_); break;
                case 0xd5: set(2, L_); break;
                case 0xd6: setInd(2, HL_); break;
                case 0xd7: set(2, A_); break;

                case 0xd8: set(3, B_); break;
                case 0xd9: set(3, C_); break;
                case 0xda: set(3, D_); break;
                case 0xdb: set(3, E_); break;
                case 0xdc: set(3, H_); break;
                case 0xdd: set(3, L_); break;
                case 0xde: setInd(3, HL_); break;
                case 0xdf: set(3, A_); break;

                case 0xe0: set(4, B_); break;
                case 0xe1: set(4, C_); break;
                case 0xe2: set(4, D_); break;
                case 0xe3: set(4, E_); break;
                case 0xe4: set(4, H_); break;
                case 0xe5: set(4, L_); break;
                case 0xe6: setInd(4, HL_); break;
                case 0xe7: set(4, A_); break;

                case 0xe8: set(5, B_); break;
                case 0xe9: set(5, C_); break;
                case 0xea: set(5, D_); break;
                case 0xeb: set(5, E_); break;
                case 0xec: set(5, H_); break;
                case 0xed: set(5, L_); break;
                case 0xee: setInd(5, HL_); break;
                case 0xef: set(5, A_); break;

                case 0xf0: set(6, B_); break;
                case 0xf1: set(6, C_); break;
                case 0xf2: set(6, D_); break;
                case 0xf3: set(6, E_); break;
                case 0xf4: set(6, H_); break;
                case 0xf5: set(6, L_); break;
                case 0xf6: setInd(6, HL_); break;
                case 0xf7: set(6, A_); break;

                case 0xf8: set(7, B_); break;
                case 0xf9: set(7, C_); break;
                case 0xfa: set(7, D_); break;
                case 0xfb: set(7, E_); break;
                case 0xfc: set(7, H_); break;
                case 0xfd: set(7, L_); break;
                case 0xfe: setInd(7, HL_); break;
                case 0xff: set(7, A_); break;
            }
        } break;
        default:
            std::ostringstream error {};
            error << "Unimplemented opcode: " << std::hex << static_cast<int>(op);
            throw std::runtime_error {error.str()};
    }
    // increment cycles
    size_t cycles_passed;
    // handle cycle timings of CB prefix instructions
    if (op == 0xcb)
    {
        // get op after prefix
        u8 op2 = read(PC_-1);
        cycles_passed = cbInstructions[op2].cycles;

    }
    // handle all other instructions
    else
    {
        cycles_passed = useBranchCycles_
            ? instructions[op].branch_cycles
            : instructions[op].cycles;
    }
    // CGB double speed mode, CPU clock ticks twice as fast
    clock_.cycles_ += (doubleSpeed_) ? cycles_passed/2 : cycles_passed;
    useBranchCycles_ = false;
    return;
}

CPUDump CPU::getDebugDump() const noexcept
{
    return {{read(PC_), read(PC_+1), read(PC_+2), read(PC_+3)}, A_, F_, B_, C_, D_, E_, H_, L_, SP_, PC_ };
}
