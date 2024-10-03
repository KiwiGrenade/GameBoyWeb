#include "CPU.hpp"
#include "utils.hpp"

void notImplemented() { Utils::warning("Opcode not implemented!"); }

ProcArray CPU::getUnprefProcArray() {
    return {
        [this] /*0x00*/ { nop(); return false; },
        [this] /*0x01*/ { notImplemented(); return false; },
        [this] /*0x02*/ { notImplemented(); return false; },
        [this] /*0x03*/ { notImplemented(); return false; },
        [this] /*0x04*/ { notImplemented(); return false; },
        [this] /*0x05*/ { notImplemented(); return false; },
        [this] /*0x06*/ { notImplemented(); return false; },
        [this] /*0x07*/ { notImplemented(); return false; },
        [this] /*0x08*/ { notImplemented(); return false; },
        [this] /*0x09*/ { notImplemented(); return false; },
        [this] /*0x0A*/ { notImplemented(); return false; },
        [this] /*0x0B*/ { notImplemented(); return false; },
        [this] /*0x0C*/ { notImplemented(); return false; },
        [this] /*0x0D*/ { notImplemented(); return false; },
        [this] /*0x0E*/ { notImplemented(); return false; },
        [this] /*0x0F*/ { notImplemented(); return false; },
        [this] /*0x10*/ { stop(); return false; },
        [this] /*0x11*/ { notImplemented(); return false; },
        [this] /*0x12*/ { notImplemented(); return false; },
        [this] /*0x13*/ { notImplemented(); return false; },
        [this] /*0x14*/ { notImplemented(); return false; },
        [this] /*0x15*/ { notImplemented(); return false; },
        [this] /*0x16*/ { notImplemented(); return false; },
        [this] /*0x17*/ { notImplemented(); return false; },
        [this] /*0x18*/ { notImplemented(); return false; },
        [this] /*0x19*/ { notImplemented(); return false; },
        [this] /*0x1A*/ { notImplemented(); return false; },
        [this] /*0x1B*/ { notImplemented(); return false; },
        [this] /*0x1C*/ { notImplemented(); return false; },
        [this] /*0x1D*/ { notImplemented(); return false; },
        [this] /*0x1E*/ { notImplemented(); return false; },
        [this] /*0x1F*/ { notImplemented(); return false; },
        [this] /*0x20*/ { notImplemented(); return false; },
        [this] /*0x21*/ { notImplemented(); return false; },
        [this] /*0x22*/ { notImplemented(); return false; },
        [this] /*0x23*/ { notImplemented(); return false; },
        [this] /*0x24*/ { notImplemented(); return false; },
        [this] /*0x25*/ { notImplemented(); return false; },
        [this] /*0x26*/ { notImplemented(); return false; },
        [this] /*0x27*/ { daa(); return false; },
        [this] /*0x28*/ { notImplemented(); return false; },
        [this] /*0x29*/ { notImplemented(); return false; },
        [this] /*0x2A*/ { notImplemented(); return false; },
        [this] /*0x2B*/ { notImplemented(); return false; },
        [this] /*0x2C*/ { notImplemented(); return false; },
        [this] /*0x2D*/ { notImplemented(); return false; },
        [this] /*0x2E*/ { notImplemented(); return false; },
        [this] /*0x2F*/ { cpl(); return false; },
        [this] /*0x30*/ { notImplemented(); return false; },
        [this] /*0x31*/ { notImplemented(); return false; },
        [this] /*0x32*/ { notImplemented(); return false; },
        [this] /*0x33*/ { notImplemented(); return false; },
        [this] /*0x34*/ { notImplemented(); return false; },
        [this] /*0x35*/ { notImplemented(); return false; },
        [this] /*0x36*/ { notImplemented(); return false; },
        [this] /*0x37*/ { return false; }, // handleFlags does the job
        [this] /*0x38*/ { notImplemented(); return false; },
        [this] /*0x39*/ { notImplemented(); return false; },
        [this] /*0x3A*/ { notImplemented(); return false; },
        [this] /*0x3B*/ { notImplemented(); return false; },
        [this] /*0x3C*/ { notImplemented(); return false; },
        [this] /*0x3D*/ { notImplemented(); return false; },
        [this] /*0x3E*/ { notImplemented(); return false; },
        [this] /*0x3F*/ { ccf(); return false; },
        [this] /*0x40*/ { ld(B_, B_); return false; },
        [this] /*0x41*/ { ld(B_, C_); return false; },
        [this] /*0x42*/ { ld(B_, D_); return false; },
        [this] /*0x43*/ { ld(B_, E_); return false; },
        [this] /*0x44*/ { ld(B_, H_); return false; },
        [this] /*0x45*/ { ld(B_, L_); return false; },
        [this] /*0x46*/ { ld(B_, memory_.read(HL_)); return false; },
        [this] /*0x47*/ { ld(B_, A_); return false; },
        [this] /*0x48*/ { ld(C_, B_); return false; },
        [this] /*0x49*/ { ld(C_, C_); return false; },
        [this] /*0x4A*/ { ld(C_, D_); return false; },
        [this] /*0x4B*/ { ld(C_, E_); return false; },
        [this] /*0x4C*/ { ld(C_, H_); return false; },
        [this] /*0x4D*/ { ld(C_, L_); return false; },
        [this] /*0x4E*/ { ld(C_, memory_.read(HL_)); return false; },
        [this] /*0x4F*/ { ld(C_, A_); return false; },
        [this] /*0x50*/ { ld(D_, B_); return false; },
        [this] /*0x51*/ { ld(D_, C_); return false; },
        [this] /*0x52*/ { ld(D_, D_); return false; },
        [this] /*0x53*/ { ld(D_, E_); return false; },
        [this] /*0x54*/ { ld(D_, H_); return false; },
        [this] /*0x55*/ { ld(D_, L_); return false; },
        [this] /*0x56*/ { ld(D_, memory_.read(HL_)); return false; },
        [this] /*0x57*/ { ld(D_, A_); return false; },
        [this] /*0x58*/ { ld(E_, B_); return false; },
        [this] /*0x59*/ { ld(E_, C_); return false; },
        [this] /*0x5A*/ { ld(E_, D_); return false; },
        [this] /*0x5B*/ { ld(E_, E_); return false; },
        [this] /*0x5C*/ { ld(E_, H_); return false; },
        [this] /*0x5D*/ { ld(E_, L_); return false; },
        [this] /*0x5E*/ { ld(E_, memory_.read(HL_)); return false; },
        [this] /*0x5F*/ { ld(E_, A_); return false; },
        [this] /*0x60*/ { ld(H_, B_); return false; },
        [this] /*0x61*/ { ld(H_, C_); return false; },
        [this] /*0x62*/ { ld(H_, D_); return false; },
        [this] /*0x63*/ { ld(H_, E_); return false; },
        [this] /*0x64*/ { ld(H_, H_); return false; },
        [this] /*0x65*/ { ld(H_, L_); return false; },
        [this] /*0x66*/ { ld(H_, memory_.read(HL_)); return false; },
        [this] /*0x67*/ { ld(H_, A_); return false; },
        [this] /*0x68*/ { ld(L_, B_); return false; },
        [this] /*0x69*/ { ld(L_, C_); return false; },
        [this] /*0x6A*/ { ld(L_, D_); return false; },
        [this] /*0x6B*/ { ld(L_, E_); return false; },
        [this] /*0x6C*/ { ld(L_, H_); return false; },
        [this] /*0x6D*/ { ld(L_, L_); return false; },
        [this] /*0x6E*/ { ld(L_, memory_.read(HL_)); return false; },
        [this] /*0x6F*/ { ld(L_, A_); return false; },
        [this] /*0x70*/ { ldd(HL_, B_); return false; },
        [this] /*0x71*/ { ldd(HL_, C_); return false; },
        [this] /*0x72*/ { ldd(HL_, D_); return false; },
        [this] /*0x73*/ { ldd(HL_, E_); return false; },
        [this] /*0x74*/ { ldd(HL_, H_); return false; },
        [this] /*0x75*/ { ldd(HL_, L_); return false; },
        [this] /*0x76*/ { halt(); return false; },
        [this] /*0x77*/ { ldd(HL_, A_); return false; },
        [this] /*0x78*/ { ld(A_, B_); return false; },
        [this] /*0x79*/ { ld(A_, C_); return false; },
        [this] /*0x7A*/ { ld(A_, D_); return false; },
        [this] /*0x7B*/ { ld(A_, E_); return false; },
        [this] /*0x7C*/ { ld(A_, H_); return false; },
        [this] /*0x7D*/ { ld(A_, L_); return false; },
        [this] /*0x7E*/ { ld(A_, memory_.read(HL_)); return false; },
        [this] /*0x7F*/ { ld(A_, A_); return false; },
        [this] /*0x80*/ { notImplemented(); return false; },
        [this] /*0x81*/ { notImplemented(); return false; },
        [this] /*0x82*/ { notImplemented(); return false; },
        [this] /*0x83*/ { notImplemented(); return false; },
        [this] /*0x84*/ { notImplemented(); return false; },
        [this] /*0x85*/ { notImplemented(); return false; },
        [this] /*0x86*/ { notImplemented(); return false; },
        [this] /*0x87*/ { notImplemented(); return false; },
        [this] /*0x88*/ { notImplemented(); return false; },
        [this] /*0x89*/ { notImplemented(); return false; },
        [this] /*0x8A*/ { notImplemented(); return false; },
        [this] /*0x8B*/ { notImplemented(); return false; },
        [this] /*0x8C*/ { notImplemented(); return false; },
        [this] /*0x8D*/ { notImplemented(); return false; },
        [this] /*0x8E*/ { notImplemented(); return false; },
        [this] /*0x8F*/ { notImplemented(); return false; },
        [this] /*0x90*/ { notImplemented(); return false; },
        [this] /*0x91*/ { notImplemented(); return false; },
        [this] /*0x92*/ { notImplemented(); return false; },
        [this] /*0x93*/ { notImplemented(); return false; },
        [this] /*0x94*/ { notImplemented(); return false; },
        [this] /*0x95*/ { notImplemented(); return false; },
        [this] /*0x96*/ { notImplemented(); return false; },
        [this] /*0x97*/ { notImplemented(); return false; },
        [this] /*0x98*/ { notImplemented(); return false; },
        [this] /*0x99*/ { notImplemented(); return false; },
        [this] /*0x9A*/ { notImplemented(); return false; },
        [this] /*0x9B*/ { notImplemented(); return false; },
        [this] /*0x9C*/ { notImplemented(); return false; },
        [this] /*0x9D*/ { notImplemented(); return false; },
        [this] /*0x9E*/ { notImplemented(); return false; },
        [this] /*0x9F*/ { notImplemented(); return false; },
        [this] /*0xA0*/ { notImplemented(); return false; },
        [this] /*0xA1*/ { notImplemented(); return false; },
        [this] /*0xA2*/ { notImplemented(); return false; },
        [this] /*0xA3*/ { notImplemented(); return false; },
        [this] /*0xA4*/ { notImplemented(); return false; },
        [this] /*0xA5*/ { notImplemented(); return false; },
        [this] /*0xA6*/ { notImplemented(); return false; },
        [this] /*0xA7*/ { notImplemented(); return false; },
        [this] /*0xA8*/ { notImplemented(); return false; },
        [this] /*0xA9*/ { notImplemented(); return false; },
        [this] /*0xAA*/ { notImplemented(); return false; },
        [this] /*0xAB*/ { notImplemented(); return false; },
        [this] /*0xAC*/ { notImplemented(); return false; },
        [this] /*0xAD*/ { notImplemented(); return false; },
        [this] /*0xAE*/ { notImplemented(); return false; },
        [this] /*0xAF*/ { notImplemented(); return false; },
        [this] /*0xB0*/ { notImplemented(); return false; },
        [this] /*0xB1*/ { notImplemented(); return false; },
        [this] /*0xB2*/ { notImplemented(); return false; },
        [this] /*0xB3*/ { notImplemented(); return false; },
        [this] /*0xB4*/ { notImplemented(); return false; },
        [this] /*0xB5*/ { notImplemented(); return false; },
        [this] /*0xB6*/ { notImplemented(); return false; },
        [this] /*0xB7*/ { notImplemented(); return false; },
        [this] /*0xB8*/ { notImplemented(); return false; },
        [this] /*0xB9*/ { notImplemented(); return false; },
        [this] /*0xBA*/ { notImplemented(); return false; },
        [this] /*0xBB*/ { notImplemented(); return false; },
        [this] /*0xBC*/ { notImplemented(); return false; },
        [this] /*0xBD*/ { notImplemented(); return false; },
        [this] /*0xBE*/ { notImplemented(); return false; },
        [this] /*0xBF*/ { notImplemented(); return false; },
        [this] /*0xC0*/ { notImplemented(); return false; },
        [this] /*0xC1*/ { notImplemented(); return false; },
        [this] /*0xC2*/ { notImplemented(); return false; },
        [this] /*0xC3*/ { notImplemented(); return false; },
        [this] /*0xC4*/ { notImplemented(); return false; },
        [this] /*0xC5*/ { notImplemented(); return false; },
        [this] /*0xC6*/ { notImplemented(); return false; },
        [this] /*0xC7*/ { notImplemented(); return false; },
        [this] /*0xC8*/ { notImplemented(); return false; },
        [this] /*0xC9*/ { notImplemented(); return false; },
        [this] /*0xCA*/ { notImplemented(); return false; },
        [this] /*0xCB*/ { notImplemented(); return false; },
        [this] /*0xCC*/ { notImplemented(); return false; },
        [this] /*0xCD*/ { notImplemented(); return false; },
        [this] /*0xCE*/ { notImplemented(); return false; },
        [this] /*0xCF*/ { notImplemented(); return false; },
        [this] /*0xD0*/ { notImplemented(); return false; },
        [this] /*0xD1*/ { notImplemented(); return false; },
        [this] /*0xD2*/ { notImplemented(); return false; },
        [this] /*0xD3*/ { notImplemented(); return false; },
        [this] /*0xD4*/ { notImplemented(); return false; },
        [this] /*0xD5*/ { notImplemented(); return false; },
        [this] /*0xD6*/ { notImplemented(); return false; },
        [this] /*0xD7*/ { notImplemented(); return false; },
        [this] /*0xD8*/ { notImplemented(); return false; },
        [this] /*0xD9*/ { notImplemented(); return false; },
        [this] /*0xDA*/ { notImplemented(); return false; },
        [this] /*0xDB*/ { notImplemented(); return false; },
        [this] /*0xDC*/ { notImplemented(); return false; },
        [this] /*0xDD*/ { notImplemented(); return false; },
        [this] /*0xDE*/ { notImplemented(); return false; },
        [this] /*0xDF*/ { notImplemented(); return false; },
        [this] /*0xE0*/ { notImplemented(); return false; },
        [this] /*0xE1*/ { notImplemented(); return false; },
        [this] /*0xE2*/ { notImplemented(); return false; },
        [this] /*0xE3*/ { notImplemented(); return false; },
        [this] /*0xE4*/ { notImplemented(); return false; },
        [this] /*0xE5*/ { notImplemented(); return false; },
        [this] /*0xE6*/ { notImplemented(); return false; },
        [this] /*0xE7*/ { notImplemented(); return false; },
        [this] /*0xE8*/ { notImplemented(); return false; },
        [this] /*0xE9*/ { notImplemented(); return false; },
        [this] /*0xEA*/ { notImplemented(); return false; },
        [this] /*0xEB*/ { notImplemented(); return false; },
        [this] /*0xEC*/ { notImplemented(); return false; },
        [this] /*0xED*/ { notImplemented(); return false; },
        [this] /*0xEE*/ { notImplemented(); return false; },
        [this] /*0xEF*/ { notImplemented(); return false; },
        [this] /*0xF0*/ { notImplemented(); return false; },
        [this] /*0xF1*/ { notImplemented(); return false; },
        [this] /*0xF2*/ { notImplemented(); return false; },
        [this] /*0xF3*/ { di(); return false; },
        [this] /*0xF4*/ { notImplemented(); return false; },
        [this] /*0xF5*/ { notImplemented(); return false; },
        [this] /*0xF6*/ { notImplemented(); return false; },
        [this] /*0xF7*/ { notImplemented(); return false; },
        [this] /*0xF8*/ { notImplemented(); return false; },
        [this] /*0xF9*/ { notImplemented(); return false; },
        [this] /*0xFA*/ { notImplemented(); return false; },
        [this] /*0xFB*/ { ei(); return false; },
        [this] /*0xFC*/ { notImplemented(); return false; },
        [this] /*0xFD*/ { notImplemented(); return false; },
        [this] /*0xFE*/ { notImplemented(); return false; },
        [this] /*0xFF*/ { notImplemented(); return false; }
    };
}

void CPU::ccf() { FlagC_.complement(); }
void CPU::scf() { /* done by handleFlags */ }
void CPU::nop() { /* do nothing */ }
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
void CPU::cpl() { A_=~A_; }
void CPU::ei() { isEISet_ = true; }
void CPU::di() { isDISet_ = true; }
void CPU::daa() {
    if(!FlagN_.val()) {
        if(FlagC_.val() || A_ > 0x99) {
            A_ += 0x60;
            FlagC_.set();
        }
        if(FlagH_.val() || (A_ & 0x0F) > 0x09)
            A_ += 0x6;
    }
    else {
        if(FlagC_.val())
            A_ -= 0x60;
        if(FlagH_.val())
            A_ -=0x6;
    }
    checkFlagZ();
}
void CPU::stop() { isStopped_ = true; }
void CPU::ld(r8& l, const r8 r) { l = r; }
void CPU::ldd(u16 addr, u8 byte) { memory_.write(byte, addr); }



