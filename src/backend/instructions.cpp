#include <cstdint>
#include "CPU.hpp"
#include "utils.hpp"

inline bool halfCheck(const u8 a, const u8 b, const u8 res)
{
    return ((a ^ b ^ res) & 0x10);
}

inline bool halfCheck16(const u16 a, const u16 b, const u16 res)
{
    return ((a ^ b ^ res) & 0x1000);
}

void CPU::ei()
{
    // EI takes effect the next cycle
    isEiSet_ = true;
}

void CPU::di()
{
    // DI takes effect the next cycle
    isDiSet_ = false;
}


void CPU::halt()
{
    if (IME_)
        isHalted_ = true;
    else
    {
        if ((memory_.read(0xffff) & memory_.read(0xff0f) & 0x1f) != 0) // HALT bug
            isHaltBug_ = true;
        else
            isHalted_ = true;
    }
}

void CPU::jr(bool cond, const int8_t d)
{
    if (cond)
    {
        pc_ += d;
        // jr is still incremented by the instruction length
        // afterwards
    }
    else
        isBranchCycle_  = true;
}

void CPU::ret(bool cond)
{
    if (cond)
    {
        pc_.lo_ = memory_.read(sp_++);
        pc_.hi_ = memory_.read(sp_++);
    }
    else
        isBranchCycle_ = true; // no path taken is shorter in cycle length
}

void CPU::jp(bool cond, const u16 adr)
{
    if (cond)
    {
        pc_ = adr;
    }
    else
        isBranchCycle_ = true;
}

void CPU::call(bool cond, const u16 adr)
{
    if (cond)
    {
        memory_.write(pc_.hi_, --sp_);
        memory_.write(pc_.lo_, --sp_);
        pc_ = adr;
    }
    else
        isBranchCycle_ = true;
}

void CPU::rst(const u8 n)
{
    memory_.write(pc_.hi_, --sp_);
    memory_.write(pc_.lo_, --sp_);
    pc_.hi_ = 0;
    pc_.lo_ = n;
}

void CPU::reti()
{
    pc_.lo_ = memory_.read(sp_++);
    pc_.hi_ = memory_.read(sp_++);
    IME_ = true;
}

// 16-bit lo_ad
void CPU::push(const u16 rp)
{
    memory_.write((rp & 0xff00) >> 8, --sp_);
    memory_.write(rp & 0xff, --sp_);
}

void CPU::pop(r16 &rp)
{
    rp.lo_ = memory_.read(sp_++);
    rp.hi_ = memory_.read(sp_++);
}

void CPU::pop_af()
{
    af_.lo_ = memory_.read(sp_++);
    af_.hi_ = memory_.read(sp_++);
    af_.lo_ &= 0xf0; // the lo_wer 4 bits of the f register are unused
}

void CPU::ldhl_sp(const int8_t d)
{
    setFlag(ZERO, false);
    setFlag(NEGATIVE, false);
    u16 res = sp_ + static_cast<u16>(d);
    if (d >= 0)
    {
        setFlag(CARRY, (sp_ & 0xff) + d > 0xff);
        setFlag(HALF, halfCheck(sp_.lo_, static_cast<u8>(d), res & 0xff));
    }
    else
    {
        setFlag(CARRY, (res & 0xff) <= (sp_ & 0xff));
        setFlag(HALF, (res & 0xf) <= (sp_ & 0xf));
    }
    hl_ = res;
}

void CPU::ldd_sp(const u16 adr)
{
    memory_.write(sp_.lo_, adr);
    memory_.write(sp_.hi_, adr+1);
}

void CPU::inc(u8 &r)
{
    setFlag(ZERO, r + 1 > 0xff);
    setFlag(HALF, halfCheck(r, 1, r+1));
    setFlag(NEGATIVE, false);
    ++r;
}

void CPU::inc_i(u16 adr)
{
    u8 b {memory_.read(adr)};
    inc(b);
    memory_.write(b, adr);
}

void CPU::dec(u8 &r)
{
    setFlag(ZERO, r - 1 == 0);
    setFlag(HALF, halfCheck(r, 1, r-1));
    setFlag(NEGATIVE, true);
    --r;
}

void CPU::dec_i(u16 adr)
{
    u8 b {memory_.read(adr)};
    dec(b);
    memory_.write(b, adr);
}

void CPU::add_sp(int8_t d)
{
    u16 res = sp_ + d;
    setFlag(ZERO, false);
    setFlag(NEGATIVE, false);
    setFlag(HALF, (res & 0xf) < (sp_ & 0xf));
    setFlag(CARRY, (res & 0xff) < (sp_ & 0xff));
    sp_ = res;
}

void CPU::daa()
{
    u8 a = af_.hi_;
    if (!getFlag(NEGATIVE))
    {
        if (getFlag(CARRY) || a > 0x99)
        {
            a += 0x60;
            setFlag(CARRY, true);
        }
        if (getFlag(HALF) || (a & 0x0f) > 0x09)
        {
            a += 0x6;
        }
    }
    else
    {
        if (getFlag(CARRY))
            a -= 0x60;
        if (getFlag(HALF))
            a -= 0x6;
    }
    setFlag(ZERO, a == 0);
    setFlag(HALF, false);
    af_.hi_ = a;
}

void CPU::scf()
{
    setFlag(CARRY, true);
    setFlag(NEGATIVE, false);
    setFlag(HALF, false);
}

void CPU::cpl()
{
    af_.hi_ = ~af_.hi_;
    setFlag(NEGATIVE, true);
    setFlag(HALF, true);
}

void CPU::ccf()
{
    setFlag(CARRY, !getFlag(CARRY));
    setFlag(NEGATIVE, false);
    setFlag(HALF, false);
}

void CPU::adc(const u8 r, bool cy)
{
    u16 res = af_.hi_ + r + cy;
    u8 res8 = af_.hi_ + r + cy;
    setFlag(ZERO, res8 == 0);
    setFlag(NEGATIVE, false);
    setFlag(HALF, halfCheck(af_.hi_, r, res8));
    setFlag(CARRY, res > 0x00ff);
    af_.hi_ = af_.hi_ + r + cy;
}

void CPU::sbc(const u8 r, bool cy)
{
    u16 res = af_.hi_ - r - cy;
    u8 res8 = af_.hi_ - r - cy;
    setFlag(ZERO, res8 == 0);
    setFlag(NEGATIVE, true);
    setFlag(HALF, halfCheck(af_.hi_, r, res8));
    setFlag(CARRY, res > 0xff);
    af_.hi_ = af_.hi_ - r - cy;
}

void CPU::andr(const u8 r)
{
    u8 res = af_.hi_ & r;
    setFlag(ZERO, res == 0);
    setFlag(NEGATIVE, false);
    setFlag(HALF, true);
    setFlag(CARRY, false);
    af_.hi_ = res;
}

void CPU::xorr(const u8 r)
{
    u8 res = af_.hi_ ^ r;
    setFlag(ZERO, res == 0);
    setFlag(NEGATIVE, false);
    setFlag(HALF, false);
    setFlag(CARRY, false);
    af_.hi_ = res;
}

void CPU::orr(const u8 r)
{
    u8 res = af_.hi_ | r;
    setFlag(ZERO, res == 0);
    setFlag(NEGATIVE, false);
    setFlag(HALF, false);
    setFlag(CARRY, false);
    af_.hi_ = res;
}

void CPU::cp(const u8 r)
{
    int16_t res = af_.hi_ - r;
    setFlag(ZERO, res == 0);
    setFlag(NEGATIVE, true);
    setFlag(HALF, halfCheck(af_.hi_, r, res & 0xff));
    setFlag(CARRY, res < 0);
}

void CPU::add(const u16 rp)
{
    setFlag(NEGATIVE, false);
    setFlag(HALF, halfCheck16(hl_, rp, hl_ + rp));
    setFlag(CARRY, (hl_ + rp) > 0xffff);
    hl_ += rp;
}

void CPU::rlc(u8 &r)
{
    bool msb {static_cast<bool>(r & 0x80)};
    r <<= 1;
    Utils::setBit(r, 0, msb);
    setFlag(ZERO, r == 0);
    setFlag(NEGATIVE, false);
    setFlag(HALF, false);
    setFlag(CARRY, msb);
}

void CPU::rlca()
{
    rlc(af_.hi_);
    setFlag(ZERO, false);
}

void CPU::rlc_i(u16 adr)
{
    u8 b {memory_.read(adr)};
    rlc(b);
    memory_.write(b, adr);
}

void CPU::rrc(u8 &r)
{
    bool lsb = r & 0x01;
    r >>= 1;
    Utils::setBit(r, 7, lsb);
    setFlag(CARRY, lsb);
    setFlag(ZERO, r == 0);
    setFlag(NEGATIVE, false);
    setFlag(HALF, false);
}

// 0x0f (rrca) modifies flags differently than cb 0f
void CPU::rrca()
{
    rrc(af_.hi_);
    setFlag(ZERO, false);
}

void CPU::rrc_i(u16 adr)
{
    u8 b {memory_.read(adr)};
    rrc(b);
    memory_.write(b, adr);
}

void CPU::rl(u8 &r)
{
    bool old_carry = getFlag(CARRY);
    setFlag(CARRY, r & 0x80);
    r <<= 1;
    Utils::setBit(r, 0, old_carry);
    setFlag(ZERO, r == 0);
    setFlag(NEGATIVE, false);
    setFlag(HALF, false);
}

void CPU::rla()
{
    rl(af_.hi_);
    setFlag(ZERO, false);
}

void CPU::rl_i(u16 adr)
{
    u8 b {memory_.read(adr)};
    rl(b);
    memory_.write(b, adr);
}

// 0x1f modifies flags differently to cb 1f
void CPU::rra()
{
    rr(af_.hi_);
    setFlag(ZERO, false);
}

void CPU::rr(u8 &r)
{
    bool old_carry = getFlag(CARRY);
    setFlag(CARRY, r & 0x01);
    r >>= 1;
    Utils::setBit(r, 7, old_carry);
    setFlag(ZERO, r == 0);
    setFlag(HALF, false);
    setFlag(NEGATIVE, false);
}

void CPU::rr_i(u16 adr)
{
    u8 b {memory_.read(adr)};
    rr(b);
    memory_.write(b, adr);
}

void CPU::sla(u8 &r)
{
    setFlag(CARRY, r & 0x80);
    r <<= 1;
    setFlag(ZERO, r == 0);
    setFlag(NEGATIVE, false);
    setFlag(HALF, false);
}

void CPU::sla_i(u16 adr)
{
    u8 b {memory_.read(adr)};
    sla(b);
    memory_.write(b, adr);
}

void CPU::sra(u8 &r)
{
    bool lsb = r & 1;
    r = (r >> 1) | (r & 0x80);
    setFlag(ZERO, r == 0);
    setFlag(NEGATIVE, false);
    setFlag(HALF, false);
    setFlag(CARRY, lsb);
}

void CPU::sra_i(u16 adr)
{
    u8 b {memory_.read(adr)};
    sra(b);
    memory_.write(b, adr);
}

void CPU::swap(u8 &r)
{
    u8 lo_w_nibble = r & 0x0f;
    u8 hi_gh_nibble = r & 0xf0;
    r = (lo_w_nibble << 4) | (hi_gh_nibble >> 4);
    setFlag(ZERO, r == 0);
    setFlag(HALF, false);
    setFlag(NEGATIVE, false);
    setFlag(CARRY, false);
}

void CPU::swap_i(u16 adr)
{
    u8 b {memory_.read(adr)};
    swap(b);
    memory_.write(b, adr);
}

void CPU::srl(u8 &r)
{
    setFlag(CARRY, r & 0x01);
    setFlag(NEGATIVE, false);
    setFlag(HALF, false);
    r >>= 1;
    setFlag(ZERO, r == 0);
}

void CPU::srl_i(u16 adr)
{
    u8 b {memory_.read(adr)};
    srl(b);
    memory_.write(b, adr);
}

void CPU::bit(const u8 n, const u8 r)
{
    setFlag(ZERO, !(r & (1 << n))); // set Z if bit is NOT set
    setFlag(NEGATIVE, false);
    setFlag(HALF, true);
}

void CPU::res(const u8 n, u8 &r)
{
    r &= ~(1UL << n);
}

void CPU::res_i(u8 n, u16 adr)
{
    u8 b {memory_.read(adr)};
    res(n, b);
    memory_.write(b, adr);
}

void CPU::set(const u8 n, u8 &r)
{
    r |= (1UL << n);
}

void CPU::set_i(u8 n, u16 adr)
{
    u8 b {memory_.read(adr)};
    set(n, b);
    memory_.write(b, adr);
}

