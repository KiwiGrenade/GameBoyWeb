#include "CPU.hpp"
#include "utils.hpp"
#include <bitset>
ProcArray CPU::getPrefProcArray() {
    return {
        [this] /*0x00*/ { rlc(B_); },
        [this] /*0x01*/ { rlc(C_); },
        [this] /*0x02*/ { rlc(D_); },
        [this] /*0x03*/ { rlc(E_); },
        [this] /*0x04*/ { rlc(H_); },
        [this] /*0x05*/ { rlc(L_); },
        [this] /*0x06*/ { rlcHL(); },
        [this] /*0x07*/ { rlc(A_); },
        [this] /*0x08*/ { rrc(B_); },
        [this] /*0x09*/ { rrc(C_); },
        [this] /*0x0A*/ { rrc(D_); },
        [this] /*0x0B*/ { rrc(E_); },
        [this] /*0x0C*/ { rrc(H_); },
        [this] /*0x0D*/ { rrc(L_); },
        [this] /*0x0E*/ { rrcHL(); },
        [this] /*0x0F*/ { rrc(A_); },
        [this] /*0x10*/ { rl(B_); },
        [this] /*0x11*/ { rl(C_); },
        [this] /*0x12*/ { rl(D_); },
        [this] /*0x13*/ { rl(E_); },
        [this] /*0x14*/ { rl(H_); },
        [this] /*0x15*/ { rl(L_); },
        [this] /*0x16*/ { rlHL(); },
        [this] /*0x17*/ { rl(A_); },
        [this] /*0x18*/ { rr(B_); },
        [this] /*0x19*/ { rr(C_); },
        [this] /*0x1A*/ { rr(D_); },
        [this] /*0x1B*/ { rr(E_); },
        [this] /*0x1C*/ { rr(H_); },
        [this] /*0x1D*/ { rr(L_); },
        [this] /*0x1E*/ { rrHL(); },
        [this] /*0x1F*/ { rr(A_); },
        [this] /*0x20*/ { sla(B_); },
        [this] /*0x21*/ { sla(C_); },
        [this] /*0x22*/ { sla(D_); },
        [this] /*0x23*/ { sla(E_); },
        [this] /*0x24*/ { sla(H_); },
        [this] /*0x25*/ { sla(L_); },
        [this] /*0x26*/ { slaHL(); },
        [this] /*0x27*/ { sla(A_); },
        [this] /*0x28*/ { sra(B_); },
        [this] /*0x29*/ { sra(C_); },
        [this] /*0x2A*/ { sra(D_); },
        [this] /*0x2B*/ { sra(E_); },
        [this] /*0x2C*/ { sra(H_); },
        [this] /*0x2D*/ { sra(L_); },
        [this] /*0x2E*/ { sraHL(); },
        [this] /*0x2F*/ { sra(A_); },
        [this] /*0x30*/ { swap(B_); },
        [this] /*0x31*/ { swap(C_); },
        [this] /*0x32*/ { swap(D_); },
        [this] /*0x33*/ { swap(E_); },
        [this] /*0x34*/ { swap(H_); },
        [this] /*0x35*/ { swap(L_); },
        [this] /*0x36*/ { swapHL(); },
        [this] /*0x37*/ { swap(A_); },
        [this] /*0x38*/ { srl(B_); },
        [this] /*0x39*/ { srl(C_); },
        [this] /*0x3A*/ { srl(D_); },
        [this] /*0x3B*/ { srl(E_); },
        [this] /*0x3C*/ { srl(H_); },
        [this] /*0x3D*/ { srl(L_); },
        [this] /*0x3E*/ { srlHL(); },
        [this] /*0x3F*/ { srl(A_); },

        // Opcodes 0x40 to 0x7F - bit instructions
        [this] /*0x40*/ { bit(0, B_); },
        [this] /*0x41*/ { bit(0, C_); },
        [this] /*0x42*/ { bit(0, D_); },
        [this] /*0x43*/ { bit(0, E_); },
        [this] /*0x44*/ { bit(0, H_); },
        [this] /*0x45*/ { bit(0, L_); },
        [this] /*0x46*/ { bitHL(0); },
        [this] /*0x47*/ { bit(0, A_); },
        [this] /*0x48*/ { bit(1, B_); },
        [this] /*0x49*/ { bit(1, C_); },
        [this] /*0x4A*/ { bit(1, D_); },
        [this] /*0x4B*/ { bit(1, E_); },
        [this] /*0x4C*/ { bit(1, H_); },
        [this] /*0x4D*/ { bit(1, L_); },
        [this] /*0x4E*/ { bitHL(1); },
        [this] /*0x4F*/ { bit(1, A_); },
        [this] /*0x50*/ { bit(2, B_); },
        [this] /*0x51*/ { bit(2, C_); },
        [this] /*0x52*/ { bit(2, D_); },
        [this] /*0x53*/ { bit(2, E_); },
        [this] /*0x54*/ { bit(2, H_); },
        [this] /*0x55*/ { bit(2, L_); },
        [this] /*0x56*/ { bitHL(2); },
        [this] /*0x57*/ { bit(2, A_); },
        [this] /*0x58*/ { bit(3, B_); },
        [this] /*0x59*/ { bit(3, C_); },
        [this] /*0x5A*/ { bit(3, D_); },
        [this] /*0x5B*/ { bit(3, E_); },
        [this] /*0x5C*/ { bit(3, H_); },
        [this] /*0x5D*/ { bit(3, L_); },
        [this] /*0x5E*/ { bitHL(3); },
        [this] /*0x5F*/ { bit(3, A_); },
        [this] /*0x60*/ { bit(4, B_); },
        [this] /*0x61*/ { bit(4, C_); },
        [this] /*0x62*/ { bit(4, D_); },
        [this] /*0x63*/ { bit(4, E_); },
        [this] /*0x64*/ { bit(4, H_); },
        [this] /*0x65*/ { bit(4, L_); },
        [this] /*0x66*/ { bitHL(4); },
        [this] /*0x67*/ { bit(4, A_); },
        [this] /*0x68*/ { bit(5, B_); },
        [this] /*0x69*/ { bit(5, C_); },
        [this] /*0x6A*/ { bit(5, D_); },
        [this] /*0x6B*/ { bit(5, E_); },
        [this] /*0x6C*/ { bit(5, H_); },
        [this] /*0x6D*/ { bit(5, L_); },
        [this] /*0x6E*/ { bitHL(5); },
        [this] /*0x6F*/ { bit(5, A_); },
        [this] /*0x70*/ { bit(6, B_); },
        [this] /*0x71*/ { bit(6, C_); },
        [this] /*0x72*/ { bit(6, D_); },
        [this] /*0x73*/ { bit(6, E_); },
        [this] /*0x74*/ { bit(6, H_); },
        [this] /*0x75*/ { bit(6, L_); },
        [this] /*0x76*/ { bitHL(6); },
        [this] /*0x77*/ { bit(6, A_); },
        [this] /*0x78*/ { bit(7, B_); },
        [this] /*0x79*/ { bit(7, C_); },
        [this] /*0x7A*/ { bit(7, D_); },
        [this] /*0x7B*/ { bit(7, E_); },
        [this] /*0x7C*/ { bit(7, H_); },
        [this] /*0x7D*/ { bit(7, L_); },
        [this] /*0x7E*/ { bitHL(7); },
        [this] /*0x7F*/ { bit(7, A_); },

        // Opcodes 0x80 to 0xBF - res instructions
        [this] /*0x80*/ { res(0, B_); },
        [this] /*0x81*/ { res(0, C_); },
        [this] /*0x82*/ { res(0, D_); },
        [this] /*0x83*/ { res(0, E_); },
        [this] /*0x84*/ { res(0, H_); },
        [this] /*0x85*/ { res(0, L_); },
        [this] /*0x86*/ { resHL(0); },
        [this] /*0x87*/ { res(0, A_); },
        [this] /*0x88*/ { res(1, B_); },
        [this] /*0x89*/ { res(1, C_); },
        [this] /*0x8A*/ { res(1, D_); },
        [this] /*0x8B*/ { res(1, E_); },
        [this] /*0x8C*/ { res(1, H_); },
        [this] /*0x8D*/ { res(1, L_); },
        [this] /*0x8E*/ { resHL(1); },
        [this] /*0x8F*/ { res(1, A_); },
        [this] /*0x90*/ { res(2, B_); },
        [this] /*0x91*/ { res(2, C_); },
        [this] /*0x92*/ { res(2, D_); },
        [this] /*0x93*/ { res(2, E_); },
        [this] /*0x94*/ { res(2, H_); },
        [this] /*0x95*/ { res(2, L_); },
        [this] /*0x96*/ { resHL(2); },
        [this] /*0x97*/ { res(2, A_); },
        [this] /*0x98*/ { res(3, B_); },
        [this] /*0x99*/ { res(3, C_); },
        [this] /*0x9A*/ { res(3, D_); },
        [this] /*0x9B*/ { res(3, E_); },
        [this] /*0x9C*/ { res(3, H_); },
        [this] /*0x9D*/ { res(3, L_); },
        [this] /*0x9E*/ { resHL(3); },
        [this] /*0x9F*/ { res(3, A_); },
        [this] /*0xA0*/ { res(4, B_); },
        [this] /*0xA1*/ { res(4, C_); },
        [this] /*0xA2*/ { res(4, D_); },
        [this] /*0xA3*/ { res(4, E_); },
        [this] /*0xA4*/ { res(4, H_); },
        [this] /*0xA5*/ { res(4, L_); },
        [this] /*0xA6*/ { resHL(4); },
        [this] /*0xA7*/ { res(4, A_); },
        [this] /*0xA8*/ { res(5, B_); },
        [this] /*0xA9*/ { res(5, C_); },
        [this] /*0xAA*/ { res(5, D_); },
        [this] /*0xAB*/ { res(5, E_); },
        [this] /*0xAC*/ { res(5, H_); },
        [this] /*0xAD*/ { res(5, L_); },
        [this] /*0xAE*/ { resHL(5); },
        [this] /*0xAF*/ { res(5, A_); },
        [this] /*0xB0*/ { res(6, B_); },
        [this] /*0xB1*/ { res(6, C_); },
        [this] /*0xB2*/ { res(6, D_); },
        [this] /*0xB3*/ { res(6, E_); },
        [this] /*0xB4*/ { res(6, H_); },
        [this] /*0xB5*/ { res(6, L_); },
        [this] /*0xB6*/ { resHL(6); },
        [this] /*0xB7*/ { res(6, A_); },
        [this] /*0xB8*/ { res(7, B_); },
        [this] /*0xB9*/ { res(7, C_); },
        [this] /*0xBA*/ { res(7, D_); },
        [this] /*0xBB*/ { res(7, E_); },
        [this] /*0xBC*/ { res(7, H_); },
        [this] /*0xBD*/ { res(7, L_); },
        [this] /*0xBE*/ { resHL(7); },
        [this] /*0xBF*/ { res(7, A_); },

        // Opcodes 0xC0 to 0xFF - set instructions
        [this] /*0xC0*/ { set(0, B_); },
        [this] /*0xC1*/ { set(0, C_); },
        [this] /*0xC2*/ { set(0, D_); },
        [this] /*0xC3*/ { set(0, E_); },
        [this] /*0xC4*/ { set(0, H_); },
        [this] /*0xC5*/ { set(0, L_); },
        [this] /*0xC6*/ { setHL(0); },
        [this] /*0xC7*/ { set(0, A_); },
        [this] /*0xC8*/ { set(1, B_); },
        [this] /*0xC9*/ { set(1, C_); },
        [this] /*0xCA*/ { set(1, D_); },
        [this] /*0xCB*/ { set(1, E_); },
        [this] /*0xCC*/ { set(1, H_); },
        [this] /*0xCD*/ { set(1, L_); },
        [this] /*0xCE*/ { setHL(1); },
        [this] /*0xCF*/ { set(1, A_); },
        [this] /*0xD0*/ { set(2, B_); },
        [this] /*0xD1*/ { set(2, C_); },
        [this] /*0xD2*/ { set(2, D_); },
        [this] /*0xD3*/ { set(2, E_); },
        [this] /*0xD4*/ { set(2, H_); },
        [this] /*0xD5*/ { set(2, L_); },
        [this] /*0xD6*/ { setHL(2); },
        [this] /*0xD7*/ { set(2, A_); },
        [this] /*0xD8*/ { set(3, B_); },
        [this] /*0xD9*/ { set(3, C_); },
        [this] /*0xDA*/ { set(3, D_); },
        [this] /*0xDB*/ { set(3, E_); },
        [this] /*0xDC*/ { set(3, H_); },
        [this] /*0xDD*/ { set(3, L_); },
        [this] /*0xDE*/ { setHL(3); },
        [this] /*0xDF*/ { set(3, A_); },
        [this] /*0xE0*/ { set(4, B_); },
        [this] /*0xE1*/ { set(4, C_); },
        [this] /*0xE2*/ { set(4, D_); },
        [this] /*0xE3*/ { set(4, E_); },
        [this] /*0xE4*/ { set(4, H_); },
        [this] /*0xE5*/ { set(4, L_); },
        [this] /*0xE6*/ { setHL(4); },
        [this] /*0xE7*/ { set(4, A_); },
        [this] /*0xE8*/ { set(5, B_); },
        [this] /*0xE9*/ { set(5, C_); },
        [this] /*0xEA*/ { set(5, D_); },
        [this] /*0xEB*/ { set(5, E_); },
        [this] /*0xEC*/ { set(5, H_); },
        [this] /*0xED*/ { set(5, L_); },
        [this] /*0xEE*/ { setHL(5); },
        [this] /*0xEF*/ { set(5, A_); },
        [this] /*0xF0*/ { set(6, B_); },
        [this] /*0xF1*/ { set(6, C_); },
        [this] /*0xF2*/ { set(6, D_); },
        [this] /*0xF3*/ { set(6, E_); },
        [this] /*0xF4*/ { set(6, H_); },
        [this] /*0xF5*/ { set(6, L_); },
        [this] /*0xF6*/ { setHL(6); },
        [this] /*0xF7*/ { set(6, A_); },
        [this] /*0xF8*/ { set(7, B_); },
        [this] /*0xF9*/ { set(7, C_); },
        [this] /*0xFA*/ { set(7, D_); },
        [this] /*0xFB*/ { set(7, E_); },
        [this] /*0xFC*/ { set(7, H_); },
        [this] /*0xFD*/ { set(7, L_); },
        [this] /*0xFE*/ { setHL(7); },
        [this] /*0xFF*/ { set(7, A_); },
    };
}

/*################### bit operations ###################*/

void CPU::swap(r8& r) {
    r8 lo = r & 0x0F;
    r8 hi = r & 0xF0;
    r = (lo << 4) | (hi >> 4);
    FlagZ_.set(!r);
}

void CPU::swapHL() {
    r8 v = fetch8(HL_);
    swap(v);
    memory_.write(v, HL_);
}

void CPU::bit(const u8 n, const r8 r) {
    FlagZ_.set(Utils::getBit(r, n) == 0);
}

void CPU::bitHL(const u8 n) {
    bit(n, fetch8(HL_));
}

void CPU::res(const u8 n, r8& r) {
    Utils::setBit(r, n, false);
}

void CPU::resHL(const u8 n) {
    r8 result = fetch8(HL_);
    res(n, result);
    memory_.write(result, HL_);
}

void CPU::set(const u8 n, r8& r) {
    Utils::setBit(r, n, true);
}

void CPU::setHL(const u8 n) {
    r8 result = fetch8(HL_);
    set(n, result);
    memory_.write(result, HL_);
}

/*################### bit shift ###################*/

void CPU::rl(r8& r) {
    bool msb = Utils::getBit(r, 7);
    r <<= 1;
    Utils::setBit(r, 0, FlagC_);
    FlagC_.set(msb);
    FlagZ_.set(!r);
}

void CPU::rlHL() {
    r8 res = fetch8(HL_);
    rl(res);
    memory_.write(res, HL_);
}

void CPU::rlc(u8& r) {
    bool msb = Utils::getBit(r, 7);
    r <<= 1;
    Utils::setBit(r, 0, msb);
    FlagC_.set(msb);
    FlagZ_.set(!r);
}

void CPU::rlcHL() {
    r8 res = fetch8(HL_);
    rlc(res);
    memory_.write(res, HL_);
}

void CPU::rr(r8& r) {
    bool lsb = Utils::getBit(r, 0);
    r >>= 1;
    Utils::setBit(r, 7, FlagC_);
    FlagC_.set(lsb);
    FlagZ_.set(!r);
}

void CPU::rrHL() {
    r8 res = fetch8(HL_);
    rr(res);
    memory_.write(res, HL_);
}

void CPU::rrc(r8& r) {
    bool lsb = Utils::getBit(r, 0);
    r >>= 1;
    Utils::setBit(r, 7, lsb);
    FlagC_.set(lsb);
    FlagZ_.set(!r);
}

void CPU::rrcHL() {
    r8 res = fetch8(HL_);
    rrc(res);
    memory_.write(res, HL_);
}

void CPU::sla(r8& r) {
    bool msb = Utils::getBit(r, 7);
    bool lsb = Utils::getBit(r, 0);
    r <<= 1;
    FlagC_.set(msb);
    FlagZ_.set(!r);
}

void CPU::slaHL() {
    u8 res = fetch8(HL_);
    sla(res);
    memory_.write(res, HL_);
}

void CPU::sra(r8& r) {
    bool lsb = Utils::getBit(r, 0);
    bool msb = Utils::getBit(r, 7);
    r >>= 1;
    Utils::setBit(r, 7, msb);
    FlagC_.set(lsb);
    FlagZ_.set(!r);
}

void CPU::sraHL() {
    u8 res = fetch8(HL_);
    sra(res);
    memory_.write(res, HL_);
}

void CPU::srl(r8& r) {
    bool lsb = Utils::getBit(r, 0);
    r >>= 1;
    FlagC_.set(lsb);
    FlagZ_.set(!r);
}

void CPU::srlHL() {
    u8 res = fetch8(HL_);
    srl(res);
    memory_.write(res, HL_);
}
