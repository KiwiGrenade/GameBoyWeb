#pragma once

#include <array>
#include "InterruptController.hpp"
#include "utils.hpp"

#include <QByteArray>
#include <memory>

#include "Cartridge.hpp"
#include "Clock.hpp"
#include "Ram.hpp"

/* ######## Memory Map #########
 *
 * 0000-3FFF 16KB ROM Bank 00 (in cartridge, fixed at bank 00)
 * 4000-7FFF 16KB ROM Bank 01..NN (in cartridge, switchable bank number)
 * 8000-9FFF 8KB Video RAM (VRAM) (switchable bank 0-1 in CGB Mode)
 * A000-BFFF 8KB External RAM (in cartridge, switchable bank, if any)
 * C000-CFFF 4KB Work RAM Bank 0 (WRAM)
 * D000-DFFF 4KB Work RAM Bank 1 (WRAM) (switchable bank 1-7 in CGB Mode)
 * E000-FDFF Same as C000-DDFF (ECHO) (typically not used)
 * FE00-FE9F Sprite Attribute Table (OAM)
 * FEA0-FEFF Not Usable
 * FF00-FF7F I/O Ports
 * FF80-FFFE High RAM (HRAM)
 * FFFF Interrupt Enable Register 
 */

class Timer;
class Joypad;
class SerialDataTransfer;
class Ppu;

class Memory {
public:
    Memory(InterruptController &ic, Timer& timer, Joypad& joypad, SerialDataTransfer& serial, Ppu& ppu, Clock& cpuClock);
    
    u8 read(const u16 addr);
    void write(const u8 byte, const u16 addr);
    void reset();
    void loadCartridge(std::shared_ptr<Cartridge>);
    void initIo();

    u8 vram_read(u8 bank, u16 addr) const;
    void vram_write(u8 byte, u8 bank, u16 addr);

    static constexpr uint32_t size_ = 0x10000;

protected:

    void oamDmaTransfer(u8 byte);
    
    std::shared_ptr<Cartridge> cartridge_;
    InterruptController &ic_;
    Timer& timer_;
    Joypad& joypad_;
    SerialDataTransfer& serial_;
    Ppu& ppu_;
    Clock& cpuClock_;

    // memory
    VRam vram_ {1};
    WorkRam wram_ {2};
    std::array<u8, 0x80> io_ {};
    std::array<u8, 0x7F> hram_ {};
    bool wroteToSram_ = false;
};
