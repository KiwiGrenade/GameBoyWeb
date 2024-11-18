#include <cstdint>
#include "CPU.hpp"

// change the nth bit of B_ to x
#define CHANGE_BIT(B_, n, x) B_ ^= (-x ^ B_) & (1UL << n)
#define CLEAR_BIT(B_, n) B_ &= ~(1UL << n)
#define SET_BIT(B_, n) B_ |= (1UL << n)

inline bool half_check(const u8 A_, const u8 B_, const u8 res)
{
    return ((A_ ^ B_ ^ res) & 0x10);
}

inline bool half_check_16(const u16 A_, const u16 B_, const u16 res)
{
    return ((A_ ^ B_ ^ res) & 0x1000);
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

void CPU::jr(bool cond, const int8_t D_)
{
    if (cond)
    {
        PC_ += D_;
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
        PC_.lo_ = read(SP_++);
        PC_.hi_ = read(SP_++);
    }
    else
        use_branch_cycles_ = true; // no path taken is shorter in cycle length
}

void CPU::jp(bool cond, const u16 adr)
{
    if (cond)
    {
        PC_ = adr;
    }
    else
        use_branch_cycles_ = true;
}

void CPU::call(bool cond, const u16 adr)
{
    if (cond)
    {
        write(PC_.hi_, --SP_);
        write(PC_.lo_, --SP_);
        PC_ = adr;
    }
    else
        use_branch_cycles_ = true;
}

void CPU::rst(const u8 n)
{
    write(PC_.hi_, --SP_);
    write(PC_.lo_, --SP_);
    PC_.hi_ = 0;
    PC_.lo_ = n;
}

void CPU::reti()
{
    PC_.lo_ = read(SP_++);
    PC_.hi_ = read(SP_++);
    ime_ = true;
}

// 16-bit load
void CPU::push(const u16 rp)
{
    write((rp & 0xff00) >> 8, --SP_);
    write(rp & 0xff, --SP_);
}

void CPU::pop(RegisterPair &rp)
{
    rp.lo_ = read(SP_++);
    rp.hi_ = read(SP_++);
}

void CPU::pop_af()
{
    AF_.lo_ = read(SP_++);
    AF_.hi_ = read(SP_++);
    AF_.lo_ &= 0xf0; // the lower 4 bits of the F_ register are unused
}

void CPU::ldhl_sp(const int8_t D_)
{
    set_flag(ZERO, false);
    set_flag(NEGATIVE, false);
    u16 res = SP_ + static_cast<u16>(D_);
    if (D_ >= 0)
    {
        set_flag(CARRY, (SP_ & 0xff) + D_ > 0xff);
        set_flag(HALF, half_check(SP_.lo_, static_cast<u8>(D_), res & 0xff));
    }
    else
    {
        set_flag(CARRY, (res & 0xff) <= (SP_ & 0xff));
        set_flag(HALF, (res & 0xf) <= (SP_ & 0xf));
    }
    HL_ = res;
}

void CPU::ldd_sp(const u16 adr)
{
    write(SP_.lo_, adr);
    write(SP_.hi_, adr+1);
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
    u8 B_ {read(adr)};
    inc(B_);
    write(B_, adr);
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
    u8 B_ {read(adr)};
    dec(B_);
    write(B_, adr);
}

void CPU::add_sp(int8_t D_)
{
    u16 res = SP_ + D_;
    set_flag(ZERO, false);
    set_flag(NEGATIVE, false);
    set_flag(HALF, (res & 0xf) < (SP_ & 0xf));
    set_flag(CARRY, (res & 0xff) < (SP_ & 0xff));
    SP_ = res;
}

void CPU::daa()
{
    u8 A_ = AF_.hi_;
    if (!get_flag(NEGATIVE))
    {
        if (get_flag(CARRY) || A_ > 0x99)
        {
            A_ += 0x60;
            set_flag(CARRY, true);
        }
        if (get_flag(HALF) || (A_ & 0x0f) > 0x09)
        {
            A_ += 0x6;
        }
    }
    else
    {
        if (get_flag(CARRY))
            A_ -= 0x60;
        if (get_flag(HALF))
            A_ -= 0x6;
    }
    set_flag(ZERO, A_ == 0);
    set_flag(HALF, false);
    AF_.hi_ = A_;
}

void CPU::scf()
{
    set_flag(CARRY, true);
    set_flag(NEGATIVE, false);
    set_flag(HALF, false);
}

void CPU::cpl()
{
    AF_.hi_ = ~AF_.hi_;
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
    u16 res = AF_.hi_ + r + cy;
    u8 res8 = AF_.hi_ + r + cy;
    set_flag(ZERO, res8 == 0);
    set_flag(NEGATIVE, false);
    set_flag(HALF, half_check(AF_.hi_, r, res8));
    set_flag(CARRY, res > 0x00ff);
    AF_.hi_ = AF_.hi_ + r + cy;
}

void CPU::sbc(const u8 r, bool cy)
{
    u16 res = AF_.hi_ - r - cy;
    u8 res8 = AF_.hi_ - r - cy;
    set_flag(ZERO, res8 == 0);
    set_flag(NEGATIVE, true);
    set_flag(HALF, half_check(AF_.hi_, r, res8));
    set_flag(CARRY, res > 0xff);
    AF_.hi_ = AF_.hi_ - r - cy;
}

void CPU::andr(const u8 r)
{
    u8 res = AF_.hi_ & r;
    set_flag(ZERO, res == 0);
    set_flag(NEGATIVE, false);
    set_flag(HALF, true);
    set_flag(CARRY, false);
    AF_.hi_ = res;
}

void CPU::xorr(const u8 r)
{
    u8 res = AF_.hi_ ^ r;
    set_flag(ZERO, res == 0);
    set_flag(NEGATIVE, false);
    set_flag(HALF, false);
    set_flag(CARRY, false);
    AF_.hi_ = res;
}

void CPU::orr(const u8 r)
{
    u8 res = AF_.hi_ | r;
    set_flag(ZERO, res == 0);
    set_flag(NEGATIVE, false);
    set_flag(HALF, false);
    set_flag(CARRY, false);
    AF_.hi_ = res;
}

void CPU::cp(const u8 r)
{
    int16_t res = AF_.hi_ - r;
    set_flag(ZERO, res == 0);
    set_flag(NEGATIVE, true);
    set_flag(HALF, half_check(AF_.hi_, r, res & 0xff));
    set_flag(CARRY, res < 0);
}

void CPU::add(const u16 rp)
{
    set_flag(NEGATIVE, false);
    set_flag(HALF, half_check_16(HL_, rp, HL_ + rp));
    set_flag(CARRY, (HL_ + rp) > 0xffff);
    HL_ += rp;
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
    rlc(AF_.hi_);
    set_flag(ZERO, false);
}

void CPU::rlc_i(u16 adr)
{
    u8 B_ {read(adr)};
    rlc(B_);
    write(B_, adr);
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
    rrc(AF_.hi_);
    set_flag(ZERO, false);
}

void CPU::rrc_i(u16 adr)
{
    u8 B_ {read(adr)};
    rrc(B_);
    write(B_, adr);
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
    rl(AF_.hi_);
    set_flag(ZERO, false);
}

void CPU::rl_i(u16 adr)
{
    u8 B_ {read(adr)};
    rl(B_);
    write(B_, adr);
}

// 0x1f modifies flags differently to cb 1f
void CPU::rra()
{
    rr(AF_.hi_);
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
    u8 B_ {read(adr)};
    rr(B_);
    write(B_, adr);
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
    u8 B_ {read(adr)};
    sla(B_);
    write(B_, adr);
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
    u8 B_ {read(adr)};
    sra(B_);
    write(B_, adr);
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
    u8 B_ {read(adr)};
    swap(B_);
    write(B_, adr);
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
    u8 B_ {read(adr)};
    srl(B_);
    write(B_, adr);
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
    u8 B_ {read(adr)};
    res(n, B_);
    write(B_, adr);
}

void CPU::set(const u8 n, u8 &r)
{
    r |= (1UL << n);
}

void CPU::set_i(u8 n, u16 adr)
{
    u8 B_ {read(adr)};
    set(n, B_);
    write(B_, adr);
}

