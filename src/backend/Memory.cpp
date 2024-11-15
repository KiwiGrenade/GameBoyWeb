#include "Memory.hpp"
#include "Timer.hpp"
#include "Joypad.hpp"
#include "SerialDataTransfer.hpp"
#include "PPU.hpp"
#include "Processor.hpp"
#include "utils.hpp"
#include <memory>
#include <unistd.h>

constexpr u16 Cartridge::romSize_;

Memory::Memory(Timer& timer, Joypad& joypad, SerialDataTransfer& serial, Ppu& ppu, Processor& cpu)
    : cartridge_(std::make_shared<Cartridge>())
    , joypad_(joypad)
    , timer_(timer)
    , serial_(serial)
    , ppu_(ppu)
    , cpu_(cpu)
    , wroteToSram_(false) {
    initIo();
}

void Memory::loadCartridge(std::shared_ptr<Cartridge> cartridge) {
    cartridge_ = cartridge;
}

void Memory::initIo() {
    io_[0x00] = 0xff;
    io_[0x05] = 0x00;   // TIMA
    io_[0x06] = 0x00;   // TMA
    io_[0x07] = 0x00;   // TAC
    io_[0x10] = 0x80;   // NR10
    io_[0x11] = 0xBF;   // NR11
    io_[0x12] = 0xF3;   // NR12
    io_[0x14] = 0xBF;   // NR14
    io_[0x16] = 0x3F;   // NR21
    io_[0x17] = 0x00;   // NR22
    io_[0x19] = 0xBF;   // NR24
    io_[0x1A] = 0x7F;   // NR30
    io_[0x1B] = 0xFF;   // NR31
    io_[0x1C] = 0x9F;   // NR32
    io_[0x1E] = 0xBF;   // NR33
    io_[0x20] = 0xFF;   // NR41
    io_[0x21] = 0x00;   // NR42
    io_[0x22] = 0x00;   // NR43
    io_[0x23] = 0xBF;   // NR44
    io_[0x24] = 0x77;   // NR50
    io_[0x25] = 0xF3;   // NR51
    io_[0x26] = 0xF1;   // NR52 (0xf1-GB, 0xF0-SGB)
    io_[0x40] = 0x91;   // LCDC
    io_[0x42] = 0x00;   // SCY
    io_[0x43] = 0x00;   // SCX
    io_[0x44] = 0x91;   // LY
    io_[0x45] = 0x00;   // LYC
    io_[0x47] = 0xFC;   // BGP
    io_[0x48] = 0xFF;   // OBP0
    io_[0x49] = 0xFF;   // OBP1
    io_[0x4A] = 0x00;   // WY
    io_[0x4B] = 0x00;   // WX
    io_[0xFF] = 0x00;   // IE
}

void Memory::write(const u8 byte, const u16 addr) {
    if(not cartridge_)
        return;

    // ROM
    if(addr < 0x8000) {
        return;
        Utils::warning("Write to ROM");
        /*cartridge_->write(byte, addr);*/
    }
    // VRAM
    else if(addr < 0xA000) {
        if(not ppu_.enabled() || ppu_.mode() != 3)
            vram_.write(byte, 0, addr - 0x8000);
    }
    // ERAM
    else if(addr < 0xC000) {
        /*eram_.write(byte, 0, addr - 0xB0000);*/
        /*cart->write(byte, addr);*/
        /*wroteToSram_ = true;*/
        /*Utils::warning("Write to ERAM");*/
    }
    // WRAM0
    else if(addr < 0xD000) {
        wram_.write(byte, 0, addr - 0xC000);
    }
    // WRAM1
    else if(addr < 0xE000) {
        wram_.write(byte, 1, addr - 0xD000);
    }
    // ECHO
    else if(addr < 0xFE00) {
        write(byte, addr - 0x2000);
    }
    // OAM
    // only accessible if PPU enabled and mode is 0 or 1
    else if(addr < 0xFEA0) {
        if(not ppu_.enabled() || ppu_.mode() < 2)
            oam_[addr - 0xFE00] = addr;
    }
    // Undefined
    else if(addr < 0xFF00) {
    }
    // IO
    else if(addr < 0xFF80) {
        // joypad
        if(addr == 0xFF00) {
            joypad_.write(byte);
        }
        // serial transfer
        else if (0xFF01 <= addr && addr <= 0xFF02) {
            switch (addr) {
                case 0xFF01:
                    serial_.setSB(byte);
                    break;
                case 0xFF02:
                    serial_.setSC(byte);
                    break;
            }
        }
        // Timer and divider
        else if (0xFF04 <= addr && addr <= 0xFF07) {
            timer_.write(byte, addr);
        }
        // Interrupts
        /*else if (0xFF0F == addr) {*/
        /*    ic_.setIF(byte);*/
        /*}*/
        // LCD - Control, Status, Position, Scrolling, Palettes
        else if (addr != 0xFF46 && 0xFF40 <= addr && addr <= 0xFF4B) {
            ppu_.write_reg(byte, addr);
        }
        // set to non zero to disable Boot ROM
        else if (0xFF46 == addr) {
            oamDmaTransfer(byte);
        }
        io_[addr - 0xFF00] = byte;
    }
    // HRAM
    else if(addr < 0xFFFF) {
        hram_[addr - 0xFF80] = byte;
    }
    // IE - InterruptController
    else if (addr == 0xFFFF) {
        ie_ = byte;
    }
}

u8 Memory::read(const u16 addr) {
    if(not cartridge_)
        return 0xFF;
    
    u8 res = 0xFF;
    
    // ROM bank
    if(addr < 0x8000) {
        res = cartridge_->read(addr);
    }
    // VRAM
    else if(addr < 0xA000) {
        // VRAM is accessible if PPU is disabled or mode isn't 3
        if(not ppu_.enabled() || ppu_.mode() != 3)
            vram_.read(0, addr-0x8000);
    }
    // ERAM
    else if(addr < 0xC000) {
        /*eram_.read(0, addr - 0xB000);*/
        /*res = cartridge_->read(addr);*/
    }
    // WRAM0
    else if(addr < 0xD000) {
        res = wram_.read(0, addr - 0xC000);
    }
    // WRAM1
    else if(addr < 0xE000) {
        res = wram_.read(1, addr - 0xD000);
    }
    // ECHO
    else if(addr < 0xFE00) {
        return read(addr - 0x2000);
    }
    // OAM
    else if(addr < 0xFEA0) {
        if(not ppu_.enabled() || ppu_.mode() < 2)
            res = oam_[addr - 0xFE00];
    }
    // undefined
    else if(addr < 0xFF00) {
        res = 0xFF;
    }
    // IO
    else if(addr < 0xFF80) {
        // Joypad
        if (addr == 0xFF00) {
            res = joypad_.read();
        }
        // serial
        else if (0xFF01 <= addr && addr <= 0xFF02) {
            switch (addr) {
                case 0xFF01:
                    return serial_.getSB();
                case 0xFF02:
                    return serial_.getSC();
            }
        }
        // timer
        else if (0xFF04 <= addr && addr <= 0xFF07) {
            res = timer_.read(addr);
        }
        // InterruptController
        /*else if (addr == 0xFF0F) {*/
        /*    res = ic_.getIF();*/
        /*}*/
        // PPU
        else if(addr != 0xFF46 && 0xFF40 <= addr && addr <= 0xFF4B) {
            res = ppu_.read_reg(addr);
        }
        else
            res = io_[addr - 0xFF00];
    }
    // HRAM
    else if(addr < 0xFFFF) {
        res = hram_[addr - 0xFF80];
    }
    // EI - InterruptController
    else if(addr == 0xFFFF) {
        res = ie_;
    }
    return res;
}

uint8_t Memory::vram_read(uint8_t bank, uint16_t a) const
{
    if (a < 0x8000 || a > 0x9fff)
        return 0xff; // not in range of VRAM
    return vram_.read(bank, a-0x8000);
}

void Memory::vram_write(uint8_t b, uint8_t bank, uint16_t a)
{
    if (a < 0x8000 || a > 0x9fff)
        return; // not in range of VRAM
    vram_.write(b, bank, a-0x8000);
}

void Memory::oamDmaTransfer(u8 byte) {
    if (byte > 0xf1) // only defined for range between 00-f1
        throw std::runtime_error {"Attempted to write value outside 00-f1 for OAM DMA transfer"};
    // make sure OAM is the correct size (not really necessary but just to make sure OAM DMA
    // doesn't access out of bounds.
    static_assert(std::tuple_size<decltype(oam_)>::value == 0xa0,
                  "OAM is incorrect size, should be 0xa0)");
    // copy from XX00-XX9f to oam (fe00-fe9f), where XXh = b
    for (uint8_t i {0}; i < 0x9f; ++i)
        oam_[i] = read(static_cast<uint16_t>(byte << 8 | i));
    // OAM DMA transfer should take 160 machine cycles (160*4 clock cycles)
    cpu_.addCycles(160*4);
}

void Memory::reset() {
    timer_.reset();
    joypad_.reset();
    serial_.reset();
}
