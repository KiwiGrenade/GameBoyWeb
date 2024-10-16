#include "CPU.hpp"
#include <cstdint>

ProcArray CPU::getUnprefProcArray() {
    return {
        [this] /*0x00*/ { nop(); },
        [this] /*0x01*/ { ld16(BC_, fetch16(PC_+1)); },
        [this] /*0x02*/ { ldd(BC_, A_); },
        [this] /*0x03*/ { inc(BC_); },
        [this] /*0x04*/ { inc(B_); },
        [this] /*0x05*/ { dec(B_); },
        [this] /*0x06*/ { ld(B_, fetch8(PC_+1)); },
        [this] /*0x07*/ { rlca(); },
        [this] /*0x08*/ { ldd16(fetch16(PC_+1), SP_); },
        [this] /*0x09*/ { add16(BC_); },
        [this] /*0x0A*/ { ld(A_, fetch8(BC_)); },
        [this] /*0x0B*/ { dec(BC_); },
        [this] /*0x0C*/ { inc(C_); },
        [this] /*0x0D*/ { dec(C_); },
        [this] /*0x0E*/ { ld(C_, fetch8(PC_+1)); },
        [this] /*0x0F*/ { rrca(); },
        [this] /*0x10*/ { stop(); },
        [this] /*0x11*/ { ld16(DE_, fetch16(PC_+1)); },
        [this] /*0x12*/ { ldd(DE_, A_); },
        [this] /*0x13*/ { inc(DE_); },
        [this] /*0x14*/ { inc(D_); },
        [this] /*0x15*/ { dec(D_); },
        [this] /*0x16*/ { ld(D_, fetch8(PC_+1)); },
        [this] /*0x17*/ { rla(); },
        [this] /*0x18*/ { jr(true, static_cast<int8_t>(fetch8(PC_+1))); },
        [this] /*0x19*/ { add16(DE_); },
        [this] /*0x1A*/ { ld(A_, fetch8(DE_)); },
        [this] /*0x1B*/ { dec(DE_); },
        [this] /*0x1C*/ { inc(E_); },
        [this] /*0x1D*/ { dec(E_); },
        [this] /*0x1E*/ { ld(E_, fetch8(PC_+1)); },
        [this] /*0x1F*/ { rra(); },
        [this] /*0x20*/ { jr(!FlagZ_, static_cast<int8_t>(fetch8(PC_+1))); },
        [this] /*0x21*/ { ld16(HL_, fetch16(PC_+1)); },
        [this] /*0x22*/ { ldd(HL_++, A_); },
        [this] /*0x23*/ { inc(HL_); },
        [this] /*0x24*/ { inc(H_); },
        [this] /*0x25*/ { dec(H_); },
        [this] /*0x26*/ { ld(H_, fetch8(PC_+1)); },
        [this] /*0x27*/ { daa(); },
        [this] /*0x28*/ { jr(FlagZ_, static_cast<int8_t>(fetch8(PC_+1))); },
        [this] /*0x29*/ { add16(HL_); },
        [this] /*0x2A*/ { ld(A_, fetch8(HL_++)); },
        [this] /*0x2B*/ { dec(HL_); },
        [this] /*0x2C*/ { inc(L_); },
        [this] /*0x2D*/ { dec(L_); },
        [this] /*0x2E*/ { ld(L_, fetch8(PC_+1)); },
        [this] /*0x2F*/ { cpl(); },
        [this] /*0x30*/ { jr(!FlagC_, static_cast<int8_t>(fetch8(PC_+1))); },
        [this] /*0x31*/ { ld16(SP_, fetch16(PC_+1)); },
        [this] /*0x32*/ { ldd(HL_--, A_); },
        [this] /*0x33*/ { inc(SP_); },
        [this] /*0x34*/ { incd(HL_); },
        [this] /*0x35*/ { decd(HL_); },
        [this] /*0x36*/ { ldd(HL_, fetch8(PC_+1)); },
        [this] /*0x37*/ { }, // handleFlags does the job
        [this] /*0x38*/ { jr(FlagC_, static_cast<int8_t>(fetch8(PC_+1))); },
        [this] /*0x39*/ { add16(SP_); },
        [this] /*0x3A*/ { ld(A_, fetch8(HL_--)); },
        [this] /*0x3B*/ { dec(SP_); },
        [this] /*0x3C*/ { inc(A_); },
        [this] /*0x3D*/ { dec(A_); },
        [this] /*0x3E*/ { ld(A_, fetch8(PC_+1)); },
        [this] /*0x3F*/ { ccf(); },
        [this] /*0x40*/ { ld(B_, B_); },
        [this] /*0x41*/ { ld(B_, C_); },
        [this] /*0x42*/ { ld(B_, D_); },
        [this] /*0x43*/ { ld(B_, E_); },
        [this] /*0x44*/ { ld(B_, H_); },
        [this] /*0x45*/ { ld(B_, L_); },
        [this] /*0x46*/ { ld(B_, fetch8(HL_)); },
        [this] /*0x47*/ { ld(B_, A_); },
        [this] /*0x48*/ { ld(C_, B_); },
        [this] /*0x49*/ { ld(C_, C_); },
        [this] /*0x4A*/ { ld(C_, D_); },
        [this] /*0x4B*/ { ld(C_, E_); },
        [this] /*0x4C*/ { ld(C_, H_); },
        [this] /*0x4D*/ { ld(C_, L_); },
        [this] /*0x4E*/ { ld(C_, fetch8(HL_)); },
        [this] /*0x4F*/ { ld(C_, A_); },
        [this] /*0x50*/ { ld(D_, B_); },
        [this] /*0x51*/ { ld(D_, C_); },
        [this] /*0x52*/ { ld(D_, D_); },
        [this] /*0x53*/ { ld(D_, E_); },
        [this] /*0x54*/ { ld(D_, H_); },
        [this] /*0x55*/ { ld(D_, L_); },
        [this] /*0x56*/ { ld(D_, fetch8(HL_)); },
        [this] /*0x57*/ { ld(D_, A_); },
        [this] /*0x58*/ { ld(E_, B_); },
        [this] /*0x59*/ { ld(E_, C_); },
        [this] /*0x5A*/ { ld(E_, D_); },
        [this] /*0x5B*/ { ld(E_, E_); },
        [this] /*0x5C*/ { ld(E_, H_); },
        [this] /*0x5D*/ { ld(E_, L_); },
        [this] /*0x5E*/ { ld(E_, fetch8(HL_)); },
        [this] /*0x5F*/ { ld(E_, A_); },
        [this] /*0x60*/ { ld(H_, B_); },
        [this] /*0x61*/ { ld(H_, C_); },
        [this] /*0x62*/ { ld(H_, D_); },
        [this] /*0x63*/ { ld(H_, E_); },
        [this] /*0x64*/ { ld(H_, H_); },
        [this] /*0x65*/ { ld(H_, L_); },
        [this] /*0x66*/ { ld(H_, fetch8(HL_)); },
        [this] /*0x67*/ { ld(H_, A_); },
        [this] /*0x68*/ { ld(L_, B_); },
        [this] /*0x69*/ { ld(L_, C_); },
        [this] /*0x6A*/ { ld(L_, D_); },
        [this] /*0x6B*/ { ld(L_, E_); },
        [this] /*0x6C*/ { ld(L_, H_); },
        [this] /*0x6D*/ { ld(L_, L_); },
        [this] /*0x6E*/ { ld(L_, fetch8(HL_)); },
        [this] /*0x6F*/ { ld(L_, A_); },
        [this] /*0x70*/ { ldd(HL_, B_); },
        [this] /*0x71*/ { ldd(HL_, C_); },
        [this] /*0x72*/ { ldd(HL_, D_); },
        [this] /*0x73*/ { ldd(HL_, E_); },
        [this] /*0x74*/ { ldd(HL_, H_); },
        [this] /*0x75*/ { ldd(HL_, L_); },
        [this] /*0x76*/ { halt(); },
        [this] /*0x77*/ { ldd(HL_, A_); },
        [this] /*0x78*/ { ld(A_, B_); },
        [this] /*0x79*/ { ld(A_, C_); },
        [this] /*0x7A*/ { ld(A_, D_); },
        [this] /*0x7B*/ { ld(A_, E_); },
        [this] /*0x7C*/ { ld(A_, H_); },
        [this] /*0x7D*/ { ld(A_, L_); },
        [this] /*0x7E*/ { ld(A_, fetch8(HL_)); },
        [this] /*0x7F*/ { ld(A_, A_); },
        [this] /*0x80*/ { add(B_); },
        [this] /*0x81*/ { add(C_); },
        [this] /*0x82*/ { add(D_); },
        [this] /*0x83*/ { add(E_); },
        [this] /*0x84*/ { add(H_); },
        [this] /*0x85*/ { add(L_); },
        [this] /*0x86*/ { add(fetch8(HL_)); },
        [this] /*0x87*/ { add(A_); },
        [this] /*0x88*/ { adc(B_); },
        [this] /*0x89*/ { adc(C_); },
        [this] /*0x8A*/ { adc(D_); },
        [this] /*0x8B*/ { adc(E_); },
        [this] /*0x8C*/ { adc(H_); },
        [this] /*0x8D*/ { adc(L_); },
        [this] /*0x8E*/ { adc(fetch8(HL_)); },
        [this] /*0x8F*/ { adc(A_); },
        [this] /*0x90*/ { sub(B_); },
        [this] /*0x91*/ { sub(C_); },
        [this] /*0x92*/ { sub(D_); },
        [this] /*0x93*/ { sub(E_); },
        [this] /*0x94*/ { sub(H_); },
        [this] /*0x95*/ { sub(L_); },
        [this] /*0x96*/ { sub(fetch8(HL_)); },
        [this] /*0x97*/ { sub(A_); },
        [this] /*0x98*/ { sbc(B_); },
        [this] /*0x99*/ { sbc(C_); },
        [this] /*0x9A*/ { sbc(D_); },
        [this] /*0x9B*/ { sbc(E_); },
        [this] /*0x9C*/ { sbc(H_); },
        [this] /*0x9D*/ { sbc(L_); },
        [this] /*0x9E*/ { sbc(fetch8(HL_)); },
        [this] /*0x9F*/ { sbc(A_); },
        [this] /*0xA0*/ { andr(B_); },
        [this] /*0xA1*/ { andr(C_); },
        [this] /*0xA2*/ { andr(D_); },
        [this] /*0xA3*/ { andr(E_); },
        [this] /*0xA4*/ { andr(H_); },
        [this] /*0xA5*/ { andr(L_); },
        [this] /*0xA6*/ { andr(fetch8(HL_)); },
        [this] /*0xA7*/ { andr(A_); },
        [this] /*0xA8*/ { xorr(B_); },
        [this] /*0xA9*/ { xorr(C_); },
        [this] /*0xAA*/ { xorr(D_); },
        [this] /*0xAB*/ { xorr(E_); },
        [this] /*0xAC*/ { xorr(H_); },
        [this] /*0xAD*/ { xorr(L_); },
        [this] /*0xAE*/ { xorr(fetch8(HL_)); },
        [this] /*0xAF*/ { xorr(A_); },
        [this] /*0xB0*/ { orr(B_); },
        [this] /*0xB1*/ { orr(C_); },
        [this] /*0xB2*/ { orr(D_); },
        [this] /*0xB3*/ { orr(E_); },
        [this] /*0xB4*/ { orr(H_); },
        [this] /*0xB5*/ { orr(L_); },
        [this] /*0xB6*/ { orr(fetch8(HL_)); },
        [this] /*0xB7*/ { orr(A_); },
        [this] /*0xB8*/ { cp(B_); },
        [this] /*0xB9*/ { cp(C_); },
        [this] /*0xBA*/ { cp(D_); },
        [this] /*0xBB*/ { cp(E_); },
        [this] /*0xBC*/ { cp(H_); },
        [this] /*0xBD*/ { cp(L_); },
        [this] /*0xBE*/ { cp(fetch8(HL_)); },
        [this] /*0xBF*/ { cp(A_); },
        [this] /*0xC0*/ { ret(!FlagZ_); },
        [this] /*0xC1*/ { pop(BC_); },
        [this] /*0xC2*/ { jp(!FlagZ_, fetch16(PC_+1)); },
        [this] /*0xC3*/ { jp(true, fetch16(PC_+1)); },
        [this] /*0xC4*/ { call(!FlagZ_, fetch16(PC_+1)); },
        [this] /*0xC5*/ { push(BC_); },
        [this] /*0xC6*/ { add(fetch8(PC_+1)); },
        [this] /*0xC7*/ { rst(0x00); },
        [this] /*0xC8*/ { ret(FlagZ_); },
        [this] /*0xC9*/ { ret(true); },
        [this] /*0xCA*/ { jp(FlagZ_, fetch16(PC_+1)); },
        [this] /*0xCB*/ { isPrefixed_ = true; },
        [this] /*0xCC*/ { call(FlagZ_, fetch16(PC_+1)); },
        [this] /*0xCD*/ { call(true, fetch16(PC_+1)); },
        [this] /*0xCE*/ { adc(fetch8(PC_+1)); },
        [this] /*0xCF*/ { rst(0x08); },
        [this] /*0xD0*/ { ret(!FlagC_); },
        [this] /*0xD1*/ { pop(DE_); },
        [this] /*0xD2*/ { jp(!FlagC_, fetch16(PC_+1)); },
        [this] /*0xD3*/ { notImplemented(); },
        [this] /*0xD4*/ { call(!FlagC_, fetch16(PC_+1)); },
        [this] /*0xD5*/ { push(DE_); },
        [this] /*0xD6*/ { sub(fetch8(PC_+1)); },
        [this] /*0xD7*/ { rst(0x10); },
        [this] /*0xD8*/ { ret(FlagC_); },
        [this] /*0xD9*/ { reti(); },
        [this] /*0xDA*/ { jp(FlagC_, fetch16(PC_+1)); },
        [this] /*0xDB*/ { notImplemented(); },
        [this] /*0xDC*/ { call(FlagC_, fetch16(PC_+1)); },
        [this] /*0xDD*/ { notImplemented(); },
        [this] /*0xDE*/ { sbc(fetch8(PC_+1)); },
        [this] /*0xDF*/ { rst(0x18); },
        [this] /*0xE0*/ { ldd(0xFF00 + fetch8(PC_+1), A_); },
        [this] /*0xE1*/ { pop(HL_); },
        [this] /*0xE2*/ { ldd(0xFF00 + C_, A_); },
        [this] /*0xE3*/ { notImplemented(); },
        [this] /*0xE4*/ { notImplemented(); },
        [this] /*0xE5*/ { push(HL_); },
        [this] /*0xE6*/ { andr(fetch8(PC_+1)); },
        [this] /*0xE7*/ { rst(0x20); },
        [this] /*0xE8*/ { addSP(); },
        [this] /*0xE9*/ { jp(true, HL_); },
        [this] /*0xEA*/ { ldd(fetch16(PC_+1), A_); },
        [this] /*0xEB*/ { notImplemented(); },
        [this] /*0xEC*/ { notImplemented(); },
        [this] /*0xED*/ { notImplemented(); },
        [this] /*0xEE*/ { xorr(fetch8(PC_+1)); },
        [this] /*0xEF*/ { rst(0x28); },
        [this] /*0xF0*/ { ld(A_, fetch8(0xFF00 + fetch8(PC_+1))); },
        [this] /*0xF1*/ { popAF(); },
        [this] /*0xF2*/ { ld(A_, fetch8(0xFF00 + C_)); },
        [this] /*0xF3*/ { di(); },
        [this] /*0xF4*/ { notImplemented(); },
        [this] /*0xF5*/ { push(AF_); },
        [this] /*0xF6*/ { orr(fetch8(PC_+1)); },
        [this] /*0xF7*/ { rst(0x30); },
        [this] /*0xF8*/ { ldHLSP(fetch8(PC_+1)); },
        [this] /*0xF9*/ { ld16(SP_, HL_); },
        [this] /*0xFA*/ { ld(A_, fetch8(fetch16(PC_+1))); },
        [this] /*0xFB*/ { ei(); },
        [this] /*0xFC*/ { notImplemented(); },
        [this] /*0xFD*/ { notImplemented(); },
        [this] /*0xFE*/ { cp(fetch8(PC_+1)); },
        [this] /*0xFF*/ { rst(0x38); }
    };
}

/*################### 8-bit arithmetic and logic ###################*/

void CPU::inc(r8& r) {
    FlagH_.set(Flag::checkH(r, 1, r+1));
    ++r;
    FlagZ_.set(!r);
}

void CPU::incd(const u16 addr) {
    u8 t = fetch8(addr);
    inc(t);
    memory_.write(t, addr);
}

void CPU::dec(r8& r) {
    FlagH_.set(Flag::checkH(r, 1, r-1));
    --r;
    FlagZ_.set(!r);
}

void CPU::decd(const u16 addr) {
    u8 b = fetch8(addr);
    dec(b);
    memory_.write(b, addr);
}

void CPU::addHelper(const u8 r, const bool addFlagC) {
    r16 res = A_ + r;
    if(addFlagC)
        res += static_cast<u16>(FlagC_);
    FlagZ_.set(!static_cast<u8>(res));
    FlagH_.set(Flag::checkH(A_, r, res.lo_));
    FlagC_.set(res > 0x00FF);
    A_ = res.lo_;
}

void CPU::add(const u8 r) {
    addHelper(r, false);
}

void CPU::adc(const u8 r) {
    addHelper(r, true);
}

void CPU::subHelper(const u8 r, const bool subFlagC) {
    r16 res = A_ - r;
    if(subFlagC)
        res -= static_cast<u16>(FlagC_);
    FlagZ_.set(!static_cast<u8>(res));
    FlagH_.set(Flag::checkH(A_, r, res.lo_));
    FlagC_.set(res > 0x00FF);
    A_ = res.lo_;
}

void CPU::sub(const u8 r) {
    subHelper(r, false);
}

void CPU::sbc(const u8 r) {
    subHelper(r, true);
}

void CPU::andr(const u8 r) {
    A_ &= r;
    FlagZ_.set(!A_);
}

void CPU::xorr(const u8 r) {
    A_ ^= r;
    FlagZ_.set(!A_);
}

void CPU::orr(const u8 r) {
    A_ |= r;
    FlagZ_.set(!A_);
}

void CPU::cp(const u8 r) {
    r16 res = A_ - r;
    FlagZ_.set(!res);
    FlagH_.set(Flag::checkH(A_, r, res.lo_));
    FlagC_.set(res < 0);
}

/*################### 16-bit arithmetic and logic ###################*/

void CPU::add16(const u16 rp) {
    uint32_t res = rp + HL_.getVal();
    FlagH_.set(Flag::checkH16(HL_, rp, res));
    FlagC_.set(res > 0xFFFF);
    HL_ += rp;
}

void CPU::dec(r16& r) {
    --r;
}

void CPU::inc(r16& r) {
    ++r;
}

/*################### bit shift ###################*/

void CPU::rla() {
    rl(A_);
}

void CPU::rlca() {
    rlc(A_);
}

void CPU::rra() {
    rr(A_);
}

void CPU::rrca() {
    rrc(A_);
}

/*################### LOAD ###################*/

void CPU::ld(r8& l, const r8 r) {
    l = r;
}

void CPU::ld16(r16& l, const u16 r) {
    l = r;
}

void CPU::ldd(u16 const addr, u8 const byte) {
    memory_.write(byte, addr);
}

void CPU::ldd16(u16 const addr, const r16 word) { 
    memory_.write(word.lo_, addr);
    memory_.write(word.hi_, addr+1);
}

/*################### JUMP and SUBROUTINES ###################*/

void CPU::jp(const bool cond, const u16 addr) {
    if(cond) {
        PC_ = addr;
        isCondMet_ = true;
        incrementPC_ = false;
    }
}

void CPU::jr(const bool cond, int8_t dest) {
    if(cond) {
        PC_ += dest;
        isCondMet_ = true;
    }
}

void CPU::call(const bool cond, const u16 addr) {
    if(cond) {
        memory_.write(PC_.hi_, --SP_);
        memory_.write(PC_.lo_, --SP_);
        PC_ = addr;
        isCondMet_ = true;
        incrementPC_ = false;
    }
}

void CPU::rst(const u8 n) {
    memory_.write(PC_.hi_, --SP_);
    memory_.write(PC_.lo_, --SP_);
    PC_.hi_ = 0;
    PC_.lo_ = n;
    incrementPC_ = false;
}

void CPU::ret(const bool cond) {
    if(cond) {
        PC_.lo_ = fetch8(SP_++);
        PC_.hi_ = fetch8(SP_++);
        isCondMet_ = true;
        incrementPC_ = false;
    }
}

void CPU::reti() {
    PC_.lo_ = fetch8(SP_++);
    PC_.hi_ = fetch8(SP_++);
    IME_ = true;
    incrementPC_ = false;
}

/*################### STACK ###################*/

void CPU::pop(r16& rp) {
    rp.lo_ = fetch8(SP_++);
    rp.hi_ = fetch8(SP_++);
}

void CPU::popAF() {
    AF_.lo_ = fetch8(SP_++);
    AF_.hi_ = fetch8(SP_++);
    AF_.lo_ &= 0xF0;
}

void CPU::push(const r16 rp) {
    memory_.write(rp.hi_, --SP_);
    memory_.write(rp.lo_, --SP_);
}

void CPU::ldHLSP(const int8_t d) {
    u16 res = SP_ + static_cast<u16>(d);
    if(d >= 0) {
        FlagC_.set((SP_ & 0xFF) + d > 0xFF);
        FlagH_.set(Flag::checkH(SP_.lo_, static_cast<u16>(d), res & 0xff));
    }
    else {
        FlagC_.set((res & 0xFF) <= (SP_ & 0xFF));
        FlagH_.set((res & 0xF) <= (SP_ & 0xF));
    }
    HL_ = res;
}

void CPU::addSP() {
    int8_t val = fetch8(PC_+1);
    FlagH_.set(Flag::checkH(SP_, val, SP_ + val));
    FlagC_.set(SP_ + val > 0x00FF);
    SP_ += val;
}

/*################### misc ###################*/

void CPU::nop() {
    /* do nothing */
}

void CPU::scf() {
    /* done by handleFlags */
}

void CPU::ccf() {
    FlagC_.complement();
}

void CPU::cpl() {
    A_ = ~A_;
}

void CPU::ei() {
    isEISet_ = true;
}

void CPU::di() {
    isDISet_ = true;
}

void CPU::stop() { isStopped_ = true; }

void CPU::halt() {
    if(IME_)
        isHalted_ = true;
    else {
        if(IE_ & IF_) {
            isHaltBug_ = true;
        }
        else
            isHalted_ = true;
    }
}

void CPU::daa() {
    if(!FlagN_) {
        if(FlagC_ || A_ > 0x99) {
            A_ += 0x60;
            FlagC_.set(true);
        }
        if(FlagH_ || (A_ & 0x0F) > 0x09)
            A_ += 0x6;
    }
    else {
        if(FlagC_)
            A_ -= 0x60;
        if(FlagH_)
            A_ -=0x6;
    }
    FlagZ_.set(A_);
}
