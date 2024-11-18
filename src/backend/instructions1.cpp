#include <cstdint>
#include "CPU.hpp"

// change the nth bit of b to x
#define CHANGE_BIT(b, n, x) b ^= (-x ^ b) & (1UL << n)
#define CLEAR_BIT(b, n) b &= ~(1UL << n)
#define SET_BIT(b, n) b |= (1UL << n)

inline bool half_check(const u8 a, const u8 b, const u8 res)
{
    return ((a ^ b ^ res) & 0x10);
}

inline bool half_check_16(const u16 a, const u16 b, const u16 res)
{
    return ((a ^ b ^ res) & 0x1000);
}

void CPU::ei()
{
    // EI takes effect the next cycle
    ei_set_ = true;
}

void CPU::di()
{
    // DI takes effect the next cycle
    di_set_ = false;
}


void CPU::halt()
{
    if (ime_)
        clock_.isHalted_ = true;
    else
    {
        if ((read(0xffff) & read(0xff0f) & 0x1f) != 0) // HALT bug
            halt_bug_ = true;
        else
            clock_.isHalted_ = true;
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
        use_branch_cycles_  = true;
}

void CPU::ret(bool cond)
{
    if (cond)
    {
        pc_.lo = read(sp_++);
        pc_.hi = read(sp_++);
    }
    else
        use_branch_cycles_ = true; // no path taken is shorter in cycle length
}

void CPU::jp(bool cond, const u16 adr)
{
    if (cond)
    {
        pc_ = adr;
    }
    else
        use_branch_cycles_ = true;
}

void CPU::call(bool cond, const u16 adr)
{
    if (cond)
    {
        write(pc_.hi, --sp_);
        write(pc_.lo, --sp_);
        pc_ = adr;
    }
    else
        use_branch_cycles_ = true;
}

void CPU::rst(const u8 n)
{
    write(pc_.hi, --sp_);
    write(pc_.lo, --sp_);
    pc_.hi = 0;
    pc_.lo = n;
}

void CPU::reti()
{
    pc_.lo = read(sp_++);
    pc_.hi = read(sp_++);
    ime_ = true;
}

// 16-bit load
void CPU::push(const u16 rp)
{
    write((rp & 0xff00) >> 8, --sp_);
    write(rp & 0xff, --sp_);
}

void CPU::pop(RegisterPair &rp)
{
    rp.lo = read(sp_++);
    rp.hi = read(sp_++);
}

void CPU::pop_af()
{
    af_.lo = read(sp_++);
    af_.hi = read(sp_++);
    af_.lo &= 0xf0; // the lower 4 bits of the f register are unused
}

void CPU::ldhl_sp(const int8_t d)
{
    set_flag(ZERO, false);
    set_flag(NEGATIVE, false);
    u16 res = sp_ + static_cast<u16>(d);
    if (d >= 0)
    {
        set_flag(CARRY, (sp_ & 0xff) + d > 0xff);
        set_flag(HALF, half_check(sp_.lo, static_cast<u8>(d), res & 0xff));
    }
    else
    {
        set_flag(CARRY, (res & 0xff) <= (sp_ & 0xff));
        set_flag(HALF, (res & 0xf) <= (sp_ & 0xf));
    }
    hl_ = res;
}

void CPU::ldd_sp(const u16 adr)
{
    write(sp_.lo, adr);
    write(sp_.hi, adr+1);
}

void CPU::inc(u8 &r)
{
    set_flag(ZERO, r + 1 > 0xff);
    set_flag(HALF, half_check(r, 1, r+1));
    set_flag(NEGATIVE, false);
    ++r;
}

void CPU::inc_i(u16 adr)
{
    u8 b {read(adr)};
    inc(b);
    write(b, adr);
}

void CPU::dec(u8 &r)
{
    set_flag(ZERO, r - 1 == 0);
    set_flag(HALF, half_check(r, 1, r-1));
    set_flag(NEGATIVE, true);
    --r;
}

void CPU::dec_i(u16 adr)
{
    u8 b {read(adr)};
    dec(b);
    write(b, adr);
}

void CPU::add_sp(int8_t d)
{
    u16 res = sp_ + d;
    set_flag(ZERO, false);
    set_flag(NEGATIVE, false);
    set_flag(HALF, (res & 0xf) < (sp_ & 0xf));
    set_flag(CARRY, (res & 0xff) < (sp_ & 0xff));
    sp_ = res;
}

void CPU::daa()
{
    u8 a = af_.hi;
    if (!get_flag(NEGATIVE))
    {
        if (get_flag(CARRY) || a > 0x99)
        {
            a += 0x60;
            set_flag(CARRY, true);
        }
        if (get_flag(HALF) || (a & 0x0f) > 0x09)
        {
            a += 0x6;
        }
    }
    else
    {
        if (get_flag(CARRY))
            a -= 0x60;
        if (get_flag(HALF))
            a -= 0x6;
    }
    set_flag(ZERO, a == 0);
    set_flag(HALF, false);
    af_.hi = a;
}

void CPU::scf()
{
    set_flag(CARRY, true);
    set_flag(NEGATIVE, false);
    set_flag(HALF, false);
}

void CPU::cpl()
{
    af_.hi = ~af_.hi;
    set_flag(NEGATIVE, true);
    set_flag(HALF, true);
}

void CPU::ccf()
{
    set_flag(CARRY, !get_flag(CARRY));
    set_flag(NEGATIVE, false);
    set_flag(HALF, false);
}

void CPU::adc(const u8 r, bool cy)
{
    u16 res = af_.hi + r + cy;
    u8 res8 = af_.hi + r + cy;
    set_flag(ZERO, res8 == 0);
    set_flag(NEGATIVE, false);
    set_flag(HALF, half_check(af_.hi, r, res8));
    set_flag(CARRY, res > 0x00ff);
    af_.hi = af_.hi + r + cy;
}

void CPU::sbc(const u8 r, bool cy)
{
    u16 res = af_.hi - r - cy;
    u8 res8 = af_.hi - r - cy;
    set_flag(ZERO, res8 == 0);
    set_flag(NEGATIVE, true);
    set_flag(HALF, half_check(af_.hi, r, res8));
    set_flag(CARRY, res > 0xff);
    af_.hi = af_.hi - r - cy;
}

void CPU::andr(const u8 r)
{
    u8 res = af_.hi & r;
    set_flag(ZERO, res == 0);
    set_flag(NEGATIVE, false);
    set_flag(HALF, true);
    set_flag(CARRY, false);
    af_.hi = res;
}

void CPU::xorr(const u8 r)
{
    u8 res = af_.hi ^ r;
    set_flag(ZERO, res == 0);
    set_flag(NEGATIVE, false);
    set_flag(HALF, false);
    set_flag(CARRY, false);
    af_.hi = res;
}

void CPU::orr(const u8 r)
{
    u8 res = af_.hi | r;
    set_flag(ZERO, res == 0);
    set_flag(NEGATIVE, false);
    set_flag(HALF, false);
    set_flag(CARRY, false);
    af_.hi = res;
}

void CPU::cp(const u8 r)
{
    int16_t res = af_.hi - r;
    set_flag(ZERO, res == 0);
    set_flag(NEGATIVE, true);
    set_flag(HALF, half_check(af_.hi, r, res & 0xff));
    set_flag(CARRY, res < 0);
}

void CPU::add(const u16 rp)
{
    set_flag(NEGATIVE, false);
    set_flag(HALF, half_check_16(hl_, rp, hl_ + rp));
    set_flag(CARRY, (hl_ + rp) > 0xffff);
    hl_ += rp;
}

void CPU::rlc(u8 &r)
{
    bool msb {static_cast<bool>(r & 0x80)};
    r <<= 1;
    CHANGE_BIT(r, 0, msb);
    set_flag(ZERO, r == 0);
    set_flag(NEGATIVE, false);
    set_flag(HALF, false);
    set_flag(CARRY, msb);
}

void CPU::rlca()
{
    rlc(af_.hi);
    set_flag(ZERO, false);
}

void CPU::rlc_i(u16 adr)
{
    u8 b {read(adr)};
    rlc(b);
    write(b, adr);
}

void CPU::rrc(u8 &r)
{
    bool lsb = r & 0x01;
    r >>= 1;
    CHANGE_BIT(r, 7, lsb);
    set_flag(CARRY, lsb);
    set_flag(ZERO, r == 0);
    set_flag(NEGATIVE, false);
    set_flag(HALF, false);
}

// 0x0f (rrca) modifies flags differently than cb 0f
void CPU::rrca()
{
    rrc(af_.hi);
    set_flag(ZERO, false);
}

void CPU::rrc_i(u16 adr)
{
    u8 b {read(adr)};
    rrc(b);
    write(b, adr);
}

void CPU::rl(u8 &r)
{
    bool old_carry = get_flag(CARRY);
    set_flag(CARRY, r & 0x80);
    r <<= 1;
    CHANGE_BIT(r, 0, old_carry);
    set_flag(ZERO, r == 0);
    set_flag(NEGATIVE, false);
    set_flag(HALF, false);
}

void CPU::rla()
{
    rl(af_.hi);
    set_flag(ZERO, false);
}

void CPU::rl_i(u16 adr)
{
    u8 b {read(adr)};
    rl(b);
    write(b, adr);
}

// 0x1f modifies flags differently to cb 1f
void CPU::rra()
{
    rr(af_.hi);
    set_flag(ZERO, false);
}

void CPU::rr(u8 &r)
{
    bool old_carry = get_flag(CARRY);
    set_flag(CARRY, r & 0x01);
    r >>= 1;
    CHANGE_BIT(r, 7, old_carry);
    set_flag(ZERO, r == 0);
    set_flag(HALF, false);
    set_flag(NEGATIVE, false);
}

void CPU::rr_i(u16 adr)
{
    u8 b {read(adr)};
    rr(b);
    write(b, adr);
}

void CPU::sla(u8 &r)
{
    set_flag(CARRY, r & 0x80);
    r <<= 1;
    set_flag(ZERO, r == 0);
    set_flag(NEGATIVE, false);
    set_flag(HALF, false);
}

void CPU::sla_i(u16 adr)
{
    u8 b {read(adr)};
    sla(b);
    write(b, adr);
}

void CPU::sra(u8 &r)
{
    bool lsb = r & 1;
    r = (r >> 1) | (r & 0x80);
    set_flag(ZERO, r == 0);
    set_flag(NEGATIVE, false);
    set_flag(HALF, false);
    set_flag(CARRY, lsb);
}

void CPU::sra_i(u16 adr)
{
    u8 b {read(adr)};
    sra(b);
    write(b, adr);
}

void CPU::swap(u8 &r)
{
    u8 low_nibble = r & 0x0f;
    u8 high_nibble = r & 0xf0;
    r = (low_nibble << 4) | (high_nibble >> 4);
    set_flag(ZERO, r == 0);
    set_flag(HALF, false);
    set_flag(NEGATIVE, false);
    set_flag(CARRY, false);
}

void CPU::swap_i(u16 adr)
{
    u8 b {read(adr)};
    swap(b);
    write(b, adr);
}

void CPU::srl(u8 &r)
{
    set_flag(CARRY, r & 0x01);
    set_flag(NEGATIVE, false);
    set_flag(HALF, false);
    r >>= 1;
    set_flag(ZERO, r == 0);
}

void CPU::srl_i(u16 adr)
{
    u8 b {read(adr)};
    srl(b);
    write(b, adr);
}

void CPU::bit(const u8 n, const u8 r)
{
    set_flag(ZERO, !(r & (1 << n))); // set Z if bit is NOT set
    set_flag(NEGATIVE, false);
    set_flag(HALF, true);
}

void CPU::res(const u8 n, u8 &r)
{
    r &= ~(1UL << n);
}

void CPU::res_i(u8 n, u16 adr)
{
    u8 b {read(adr)};
    res(n, b);
    write(b, adr);
}

void CPU::set(const u8 n, u8 &r)
{
    r |= (1UL << n);
}

void CPU::set_i(u8 n, u16 adr)
{
    u8 b {read(adr)};
    set(n, b);
    write(b, adr);
}

