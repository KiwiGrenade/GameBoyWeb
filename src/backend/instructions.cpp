#include <cstdint>
#include "CPU.hpp"
#include "utils.hpp"

inline bool half_check(const u8 A_, const u8 b, const u8 res) {
    return ((A_ ^ b ^ res) & 0x10);
}

inline bool half_check_16(const u16 A_, const u16 b, const u16 res) {
    return ((A_ ^ b ^ res) & 0x1000);
}

void CPU::ei() {
    // EI takes effect the next cycle
    eiSet_ = true;
}

void CPU::di() {
    // DI takes effect the next cycle
    diSet_ = false;
}

void CPU::halt() {
    if (IME_)
        clock_.isHalted_ = true;
}

void CPU::jr(bool cond, const int8_t d) {
    if (cond)
        PC_ += d;
    else
        useBranchCycles_ = true;
}

void CPU::ret(bool cond) {
    if (cond) {
        PC_.lo_ = read(SP_++);
        PC_.hi_ = read(SP_++);
    } else
        useBranchCycles_ = true;
}

void CPU::jp(bool cond, const u16 adr) {
    if (cond) {
        PC_ = adr;
    } else
        useBranchCycles_ = true;
}

void CPU::call(bool cond, const u16 adr) {
    if (cond) {
        write(PC_.hi_, --SP_);
        write(PC_.lo_, --SP_);
        PC_ = adr;
    } else
        useBranchCycles_ = true;
}

void CPU::rst(const u8 n) {
    write(PC_.hi_, --SP_);
    write(PC_.lo_, --SP_);
    PC_.hi_ = 0;
    PC_.lo_ = n;
}

void CPU::reti() {
    PC_.lo_ = read(SP_++);
    PC_.hi_ = read(SP_++);
    IME_ = true;
}

// 16-bit load
void CPU::push(const u16 rp) {
    write((rp & 0xff00) >> 8, --SP_);
    write(rp & 0xff, --SP_);
}

void CPU::pop(r16 &rp) {
    rp.lo_ = read(SP_++);
    rp.hi_ = read(SP_++);
}

void CPU::popAf() {
    AF_.lo_ = read(SP_++);
    AF_.hi_ = read(SP_++);
    AF_.lo_ &= 0xf0;
}

void CPU::ldhlSp(const int8_t d) {
    setFlag(ZERO, false);
    setFlag(NEGATIVE, false);
    u16 res = SP_ + static_cast<u16>(d);
    if (d >= 0) {
        setFlag(CARRY, (SP_ & 0xff) + d > 0xff);
        setFlag(HALF, half_check(SP_.lo_, static_cast<u8>(d), res & 0xff));
    } else {
        setFlag(CARRY, (res & 0xff) <= (SP_ & 0xff));
        setFlag(HALF, (res & 0xf) <= (SP_ & 0xf));
    }
    HL_ = res;
}

void CPU::lddSp(const u16 adr) {
    write(SP_.lo_, adr);
    write(SP_.hi_, adr + 1);
}

void CPU::inc(u8 &r) {
    setFlag(ZERO, r + 1 > 0xff);
    setFlag(HALF, half_check(r, 1, r + 1));
    setFlag(NEGATIVE, false);
    ++r;
}

void CPU::incInd(u16 adr) {
    u8 b{read(adr)};
    inc(b);
    write(b, adr);
}

void CPU::dec(u8 &r) {
    setFlag(ZERO, r - 1 == 0);
    setFlag(HALF, half_check(r, 1, r - 1));
    setFlag(NEGATIVE, true);
    --r;
}

void CPU::decInd(u16 adr) {
    u8 b{read(adr)};
    dec(b);
    write(b, adr);
}

void CPU::add_sp(int8_t d) {
    u16 res = SP_ + d;
    setFlag(ZERO, false);
    setFlag(NEGATIVE, false);
    setFlag(HALF, (res & 0xf) < (SP_ & 0xf));
    setFlag(CARRY, (res & 0xff) < (SP_ & 0xff));
    SP_ = res;
}

void CPU::daa() {
    u8 A_ = AF_.hi_;
    if (!getFlag(NEGATIVE)) {
        if (getFlag(CARRY) || A_ > 0x99) {
            A_ += 0x60;
            setFlag(CARRY, true);
        }
        if (getFlag(HALF) || (A_ & 0x0f) > 0x09) {
            A_ += 0x6;
        }
    } else {
        if (getFlag(CARRY))
            A_ -= 0x60;
        if (getFlag(HALF))
            A_ -= 0x6;
    }
    setFlag(ZERO, A_ == 0);
    setFlag(HALF, false);
    AF_.hi_ = A_;
}

void CPU::scf() {
    setFlag(CARRY, true);
    setFlag(NEGATIVE, false);
    setFlag(HALF, false);
}

void CPU::cpl() {
    AF_.hi_ = ~AF_.hi_;
    setFlag(NEGATIVE, true);
    setFlag(HALF, true);
}

void CPU::ccf() {
    setFlag(CARRY, !getFlag(CARRY));
    setFlag(NEGATIVE, false);
    setFlag(HALF, false);
}

void CPU::adc(const u8 r, bool cy) {
    u16 res = AF_.hi_ + r + cy;
    u8 res8 = AF_.hi_ + r + cy;
    setFlag(ZERO, res8 == 0);
    setFlag(NEGATIVE, false);
    setFlag(HALF, half_check(AF_.hi_, r, res8));
    setFlag(CARRY, res > 0x00ff);
    AF_.hi_ = AF_.hi_ + r + cy;
}

void CPU::sbc(const u8 r, bool cy) {
    u16 res = AF_.hi_ - r - cy;
    u8 res8 = AF_.hi_ - r - cy;
    setFlag(ZERO, res8 == 0);
    setFlag(NEGATIVE, true);
    setFlag(HALF, half_check(AF_.hi_, r, res8));
    setFlag(CARRY, res > 0xff);
    AF_.hi_ = AF_.hi_ - r - cy;
}

void CPU::andr(const u8 r) {
    u8 res = AF_.hi_ & r;
    setFlag(ZERO, res == 0);
    setFlag(NEGATIVE, false);
    setFlag(HALF, true);
    setFlag(CARRY, false);
    AF_.hi_ = res;
}

void CPU::xorr(const u8 r) {
    u8 res = AF_.hi_ ^ r;
    setFlag(ZERO, res == 0);
    setFlag(NEGATIVE, false);
    setFlag(HALF, false);
    setFlag(CARRY, false);
    AF_.hi_ = res;
}

void CPU::orr(const u8 r) {
    u8 res = AF_.hi_ | r;
    setFlag(ZERO, res == 0);
    setFlag(NEGATIVE, false);
    setFlag(HALF, false);
    setFlag(CARRY, false);
    AF_.hi_ = res;
}

void CPU::cp(const u8 r) {
    int16_t res = AF_.hi_ - r;
    setFlag(ZERO, res == 0);
    setFlag(NEGATIVE, true);
    setFlag(HALF, half_check(AF_.hi_, r, res & 0xff));
    setFlag(CARRY, res < 0);
}

void CPU::add(const u16 rp) {
    setFlag(NEGATIVE, false);
    setFlag(HALF, half_check_16(HL_, rp, HL_ + rp));
    setFlag(CARRY, (HL_ + rp) > 0xffff);
    HL_ += rp;
}

void CPU::rlc(u8 &r) {
    bool msb{static_cast<bool>(r & 0x80)};
    r <<= 1;
    Utils::setBit(r, 0, msb);
    setFlag(ZERO, r == 0);
    setFlag(NEGATIVE, false);
    setFlag(HALF, false);
    setFlag(CARRY, msb);
}

void CPU::rlca() {
    rlc(AF_.hi_);
    setFlag(ZERO, false);
}

void CPU::rlcInd(u16 adr) {
    u8 b{read(adr)};
    rlc(b);
    write(b, adr);
}

void CPU::rrc(u8 &r) {
    bool lsb = r & 0x01;
    r >>= 1;
    Utils::setBit(r, 7, lsb);
    setFlag(CARRY, lsb);
    setFlag(ZERO, r == 0);
    setFlag(NEGATIVE, false);
    setFlag(HALF, false);
}

void CPU::rrca() {
    rrc(AF_.hi_);
    setFlag(ZERO, false);
}

void CPU::rrcInd(u16 adr) {
    u8 b{read(adr)};
    rrc(b);
    write(b, adr);
}

void CPU::rl(u8 &r) {
    bool old_carry = getFlag(CARRY);
    setFlag(CARRY, r & 0x80);
    r <<= 1;
    Utils::setBit(r, 0, old_carry);
    setFlag(ZERO, r == 0);
    setFlag(NEGATIVE, false);
    setFlag(HALF, false);
}

void CPU::rla() {
    rl(AF_.hi_);
    setFlag(ZERO, false);
}

void CPU::rlInd(u16 adr) {
    u8 b{read(adr)};
    rl(b);
    write(b, adr);
}

void CPU::rra() {
    rr(AF_.hi_);
    setFlag(ZERO, false);
}

void CPU::rr(u8 &r) {
    bool oldCarry = getFlag(CARRY);
    setFlag(CARRY, r & 0x01);
    r >>= 1;
    Utils::setBit(r, 7, oldCarry);
    setFlag(ZERO, r == 0);
    setFlag(HALF, false);
    setFlag(NEGATIVE, false);
}

void CPU::rrInd(u16 adr) {
    u8 b{read(adr)};
    rr(b);
    write(b, adr);
}

void CPU::sla(u8 &r) {
    setFlag(CARRY, r & 0x80);
    r <<= 1;
    setFlag(ZERO, r == 0);
    setFlag(NEGATIVE, false);
    setFlag(HALF, false);
}

void CPU::slaInd(u16 adr) {
    u8 b{read(adr)};
    sla(b);
    write(b, adr);
}

void CPU::sra(u8 &r) {
    bool lsb = r & 1;
    r = (r >> 1) | (r & 0x80);
    setFlag(ZERO, r == 0);
    setFlag(NEGATIVE, false);
    setFlag(HALF, false);
    setFlag(CARRY, lsb);
}

void CPU::sraInd(u16 adr) {
    u8 b{read(adr)};
    sra(b);
    write(b, adr);
}

void CPU::swap(u8 &r) {
    u8 lo = r & 0x0f;
    u8 hi = r & 0xf0;
    r = (lo << 4) | (hi >> 4);
    setFlag(ZERO, r == 0);
    setFlag(HALF, false);
    setFlag(NEGATIVE, false);
    setFlag(CARRY, false);
}

void CPU::swapInd(u16 adr) {
    u8 b{read(adr)};
    swap(b);
    write(b, adr);
}

void CPU::srl(u8 &r) {
    setFlag(CARRY, r & 0x01);
    setFlag(NEGATIVE, false);
    setFlag(HALF, false);
    r >>= 1;
    setFlag(ZERO, r == 0);
}

void CPU::srlInd(u16 adr) {
    u8 b{read(adr)};
    srl(b);
    write(b, adr);
}

void CPU::bit(const u8 n, const u8 r) {
    setFlag(ZERO, !(r & (1 << n)));
    setFlag(NEGATIVE, false);
    setFlag(HALF, true);
}

void CPU::res(const u8 n, u8 &r) {
    r &= ~(1UL << n);
}

void CPU::resInd(u8 n, u16 adr) {
    u8 b{read(adr)};
    res(n, b);
    write(b, adr);
}

void CPU::set(const u8 n, u8 &r) {
    r |= (1UL << n);
}

void CPU::setInd(u8 n, u16 adr) {
    u8 b{read(adr)};
    set(n, b);
    write(b, adr);
}

