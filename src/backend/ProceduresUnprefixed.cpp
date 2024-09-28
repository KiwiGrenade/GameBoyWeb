#include "CPU.hpp"

ProcArray CPU::getUnprefProcArray() {
    return {
        [this] /*0x00*/ { nop(); return false; },
        [this] /*0x01*/ { nop(); return false; },
        [this] /*0x02*/ { nop(); return false; },
        [this] /*0x03*/ { nop(); return false; },
        [this] /*0x04*/ { nop(); return false; },
        [this] /*0x05*/ { nop(); return false; },
        [this] /*0x06*/ { nop(); return false; },
        [this] /*0x07*/ { nop(); return false; },
        [this] /*0x08*/ { nop(); return false; },
        [this] /*0x09*/ { nop(); return false; },
        [this] /*0x0A*/ { nop(); return false; },
        [this] /*0x0B*/ { nop(); return false; },
        [this] /*0x0C*/ { nop(); return false; },
        [this] /*0x0D*/ { nop(); return false; },
        [this] /*0x0E*/ { nop(); return false; },
        [this] /*0x0F*/ { nop(); return false; },
        [this] /*0x10*/ { nop(); return false; },
        [this] /*0x11*/ { nop(); return false; },
        [this] /*0x12*/ { nop(); return false; },
        [this] /*0x13*/ { nop(); return false; },
        [this] /*0x14*/ { nop(); return false; },
        [this] /*0x15*/ { nop(); return false; },
        [this] /*0x16*/ { nop(); return false; },
        [this] /*0x17*/ { nop(); return false; },
        [this] /*0x18*/ { nop(); return false; },
        [this] /*0x19*/ { nop(); return false; },
        [this] /*0x1A*/ { nop(); return false; },
        [this] /*0x1B*/ { nop(); return false; },
        [this] /*0x1C*/ { nop(); return false; },
        [this] /*0x1D*/ { nop(); return false; },
        [this] /*0x1E*/ { nop(); return false; },
        [this] /*0x1F*/ { nop(); return false; },
        [this] /*0x20*/ { nop(); return false; },
        [this] /*0x21*/ { nop(); return false; },
        [this] /*0x22*/ { nop(); return false; },
        [this] /*0x23*/ { nop(); return false; },
        [this] /*0x24*/ { nop(); return false; },
        [this] /*0x25*/ { nop(); return false; },
        [this] /*0x26*/ { nop(); return false; },
        [this] /*0x27*/ { nop(); return false; },
        [this] /*0x28*/ { nop(); return false; },
        [this] /*0x29*/ { nop(); return false; },
        [this] /*0x2A*/ { nop(); return false; },
        [this] /*0x2B*/ { nop(); return false; },
        [this] /*0x2C*/ { nop(); return false; },
        [this] /*0x2D*/ { nop(); return false; },
        [this] /*0x2E*/ { nop(); return false; },
        [this] /*0x2F*/ { cpl(); return false; },
        [this] /*0x30*/ { nop(); return false; },
        [this] /*0x31*/ { nop(); return false; },
        [this] /*0x32*/ { nop(); return false; },
        [this] /*0x33*/ { nop(); return false; },
        [this] /*0x34*/ { nop(); return false; },
        [this] /*0x35*/ { nop(); return false; },
        [this] /*0x36*/ { nop(); return false; },
        [this] /*0x37*/ { nop(); return false; },
        [this] /*0x38*/ { nop(); return false; },
        [this] /*0x39*/ { nop(); return false; },
        [this] /*0x3A*/ { nop(); return false; },
        [this] /*0x3B*/ { nop(); return false; },
        [this] /*0x3C*/ { nop(); return false; },
        [this] /*0x3D*/ { nop(); return false; },
        [this] /*0x3E*/ { nop(); return false; },
        [this] /*0x3F*/ { ccf(); return false; },
        [this] /*0x40*/ { nop(); return false; },
        [this] /*0x41*/ { nop(); return false; },
        [this] /*0x42*/ { nop(); return false; },
        [this] /*0x43*/ { nop(); return false; },
        [this] /*0x44*/ { nop(); return false; },
        [this] /*0x45*/ { nop(); return false; },
        [this] /*0x46*/ { nop(); return false; },
        [this] /*0x47*/ { nop(); return false; },
        [this] /*0x48*/ { nop(); return false; },
        [this] /*0x49*/ { nop(); return false; },
        [this] /*0x4A*/ { nop(); return false; },
        [this] /*0x4B*/ { nop(); return false; },
        [this] /*0x4C*/ { nop(); return false; },
        [this] /*0x4D*/ { nop(); return false; },
        [this] /*0x4E*/ { nop(); return false; },
        [this] /*0x4F*/ { nop(); return false; },
        [this] /*0x50*/ { nop(); return false; },
        [this] /*0x51*/ { nop(); return false; },
        [this] /*0x52*/ { nop(); return false; },
        [this] /*0x53*/ { nop(); return false; },
        [this] /*0x54*/ { nop(); return false; },
        [this] /*0x55*/ { nop(); return false; },
        [this] /*0x56*/ { nop(); return false; },
        [this] /*0x57*/ { nop(); return false; },
        [this] /*0x58*/ { nop(); return false; },
        [this] /*0x59*/ { nop(); return false; },
        [this] /*0x5A*/ { nop(); return false; },
        [this] /*0x5B*/ { nop(); return false; },
        [this] /*0x5C*/ { nop(); return false; },
        [this] /*0x5D*/ { nop(); return false; },
        [this] /*0x5E*/ { nop(); return false; },
        [this] /*0x5F*/ { nop(); return false; },
        [this] /*0x60*/ { nop(); return false; },
        [this] /*0x61*/ { nop(); return false; },
        [this] /*0x62*/ { nop(); return false; },
        [this] /*0x63*/ { nop(); return false; },
        [this] /*0x64*/ { nop(); return false; },
        [this] /*0x65*/ { nop(); return false; },
        [this] /*0x66*/ { nop(); return false; },
        [this] /*0x67*/ { scf(); return false; },
        [this] /*0x68*/ { nop(); return false; },
        [this] /*0x69*/ { nop(); return false; },
        [this] /*0x6A*/ { nop(); return false; },
        [this] /*0x6B*/ { nop(); return false; },
        [this] /*0x6C*/ { nop(); return false; },
        [this] /*0x6D*/ { nop(); return false; },
        [this] /*0x6E*/ { nop(); return false; },
        [this] /*0x6F*/ { nop(); return false; },
        [this] /*0x70*/ { nop(); return false; },
        [this] /*0x71*/ { nop(); return false; },
        [this] /*0x72*/ { nop(); return false; },
        [this] /*0x73*/ { nop(); return false; },
        [this] /*0x74*/ { nop(); return false; },
        [this] /*0x75*/ { nop(); return false; },
        [this] /*0x76*/ { nop(); return false; },
        [this] /*0x77*/ { ccf(); return false; },
        [this] /*0x78*/ { nop(); return false; },
        [this] /*0x79*/ { nop(); return false; },
        [this] /*0x7A*/ { nop(); return false; },
        [this] /*0x7B*/ { nop(); return false; },
        [this] /*0x7C*/ { nop(); return false; },
        [this] /*0x7D*/ { nop(); return false; },
        [this] /*0x7E*/ { nop(); return false; },
        [this] /*0x7F*/ { nop(); return false; },
        [this] /*0x80*/ { nop(); return false; },
        [this] /*0x81*/ { nop(); return false; },
        [this] /*0x82*/ { nop(); return false; },
        [this] /*0x83*/ { nop(); return false; },
        [this] /*0x84*/ { nop(); return false; },
        [this] /*0x85*/ { nop(); return false; },
        [this] /*0x86*/ { nop(); return false; },
        [this] /*0x87*/ { nop(); return false; },
        [this] /*0x88*/ { nop(); return false; },
        [this] /*0x89*/ { nop(); return false; },
        [this] /*0x8A*/ { nop(); return false; },
        [this] /*0x8B*/ { nop(); return false; },
        [this] /*0x8C*/ { nop(); return false; },
        [this] /*0x8D*/ { nop(); return false; },
        [this] /*0x8E*/ { nop(); return false; },
        [this] /*0x8F*/ { nop(); return false; },
        [this] /*0x90*/ { nop(); return false; },
        [this] /*0x91*/ { nop(); return false; },
        [this] /*0x92*/ { nop(); return false; },
        [this] /*0x93*/ { nop(); return false; },
        [this] /*0x94*/ { nop(); return false; },
        [this] /*0x95*/ { nop(); return false; },
        [this] /*0x96*/ { nop(); return false; },
        [this] /*0x97*/ { nop(); return false; },
        [this] /*0x98*/ { nop(); return false; },
        [this] /*0x99*/ { nop(); return false; },
        [this] /*0x9A*/ { nop(); return false; },
        [this] /*0x9B*/ { nop(); return false; },
        [this] /*0x9C*/ { nop(); return false; },
        [this] /*0x9D*/ { nop(); return false; },
        [this] /*0x9E*/ { nop(); return false; },
        [this] /*0x9F*/ { nop(); return false; },
        [this] /*0xA0*/ { nop(); return false; },
        [this] /*0xA1*/ { nop(); return false; },
        [this] /*0xA2*/ { nop(); return false; },
        [this] /*0xA3*/ { nop(); return false; },
        [this] /*0xA4*/ { nop(); return false; },
        [this] /*0xA5*/ { nop(); return false; },
        [this] /*0xA6*/ { nop(); return false; },
        [this] /*0xA7*/ { nop(); return false; },
        [this] /*0xA8*/ { nop(); return false; },
        [this] /*0xA9*/ { nop(); return false; },
        [this] /*0xAA*/ { nop(); return false; },
        [this] /*0xAB*/ { nop(); return false; },
        [this] /*0xAC*/ { nop(); return false; },
        [this] /*0xAD*/ { nop(); return false; },
        [this] /*0xAE*/ { nop(); return false; },
        [this] /*0xAF*/ { nop(); return false; },
        [this] /*0xB0*/ { nop(); return false; },
        [this] /*0xB1*/ { nop(); return false; },
        [this] /*0xB2*/ { nop(); return false; },
        [this] /*0xB3*/ { nop(); return false; },
        [this] /*0xB4*/ { nop(); return false; },
        [this] /*0xB5*/ { nop(); return false; },
        [this] /*0xB6*/ { nop(); return false; },
        [this] /*0xB7*/ { nop(); return false; },
        [this] /*0xB8*/ { nop(); return false; },
        [this] /*0xB9*/ { nop(); return false; },
        [this] /*0xBA*/ { nop(); return false; },
        [this] /*0xBB*/ { nop(); return false; },
        [this] /*0xBC*/ { nop(); return false; },
        [this] /*0xBD*/ { nop(); return false; },
        [this] /*0xBE*/ { nop(); return false; },
        [this] /*0xBF*/ { nop(); return false; },
        [this] /*0xC0*/ { nop(); return false; },
        [this] /*0xC1*/ { nop(); return false; },
        [this] /*0xC2*/ { nop(); return false; },
        [this] /*0xC3*/ { nop(); return false; },
        [this] /*0xC4*/ { nop(); return false; },
        [this] /*0xC5*/ { nop(); return false; },
        [this] /*0xC6*/ { nop(); return false; },
        [this] /*0xC7*/ { nop(); return false; },
        [this] /*0xC8*/ { nop(); return false; },
        [this] /*0xC9*/ { nop(); return false; },
        [this] /*0xCA*/ { nop(); return false; },
        [this] /*0xCB*/ { nop(); return false; },
        [this] /*0xCC*/ { nop(); return false; },
        [this] /*0xCD*/ { nop(); return false; },
        [this] /*0xCE*/ { nop(); return false; },
        [this] /*0xCF*/ { nop(); return false; },
        [this] /*0xD0*/ { nop(); return false; },
        [this] /*0xD1*/ { nop(); return false; },
        [this] /*0xD2*/ { nop(); return false; },
        [this] /*0xD3*/ { nop(); return false; },
        [this] /*0xD4*/ { nop(); return false; },
        [this] /*0xD5*/ { nop(); return false; },
        [this] /*0xD6*/ { nop(); return false; },
        [this] /*0xD7*/ { nop(); return false; },
        [this] /*0xD8*/ { nop(); return false; },
        [this] /*0xD9*/ { nop(); return false; },
        [this] /*0xDA*/ { nop(); return false; },
        [this] /*0xDB*/ { nop(); return false; },
        [this] /*0xDC*/ { nop(); return false; },
        [this] /*0xDD*/ { nop(); return false; },
        [this] /*0xDE*/ { nop(); return false; },
        [this] /*0xDF*/ { nop(); return false; },
        [this] /*0xE0*/ { nop(); return false; },
        [this] /*0xE1*/ { nop(); return false; },
        [this] /*0xE2*/ { nop(); return false; },
        [this] /*0xE3*/ { nop(); return false; },
        [this] /*0xE4*/ { nop(); return false; },
        [this] /*0xE5*/ { nop(); return false; },
        [this] /*0xE6*/ { nop(); return false; },
        [this] /*0xE7*/ { nop(); return false; },
        [this] /*0xE8*/ { nop(); return false; },
        [this] /*0xE9*/ { nop(); return false; },
        [this] /*0xEA*/ { nop(); return false; },
        [this] /*0xEB*/ { nop(); return false; },
        [this] /*0xEC*/ { nop(); return false; },
        [this] /*0xED*/ { nop(); return false; },
        [this] /*0xEE*/ { nop(); return false; },
        [this] /*0xEF*/ { nop(); return false; },
        [this] /*0xF0*/ { nop(); return false; },
        [this] /*0xF1*/ { nop(); return false; },
        [this] /*0xF2*/ { nop(); return false; },
        [this] /*0xF3*/ { nop(); return false; },
        [this] /*0xF4*/ { nop(); return false; },
        [this] /*0xF5*/ { nop(); return false; },
        [this] /*0xF6*/ { nop(); return false; },
        [this] /*0xF7*/ { nop(); return false; },
        [this] /*0xF8*/ { nop(); return false; },
        [this] /*0xF9*/ { nop(); return false; },
        [this] /*0xFA*/ { nop(); return false; },
        [this] /*0xFB*/ { nop(); return false; },
        [this] /*0xFC*/ { nop(); return false; },
        [this] /*0xFD*/ { nop(); return false; },
        [this] /*0xFE*/ { nop(); return false; },
        [this] /*0xFF*/ { nop(); return false; }
    };
}

void CPU::ccf() { FlagC_.complement(); }
void CPU::scf() { /* done by handleFlags */ }
void CPU::nop() { return; }
void CPU::cpl() { A_=~A_; }