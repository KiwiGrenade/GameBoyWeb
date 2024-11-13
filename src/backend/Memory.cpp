#include "Memory.hpp"
#include "PPU.hpp"
#include "CPU.hpp"
#include "Ram.hpp"
#include "Cartridge.hpp"
#include "utils.hpp"
#include <memory>
#include <unistd.h>

constexpr u16 Cartridge::romSize_;

Memory::Memory(InterruptController& ic, Timer& timer, Joypad& joypad, SerialDataTransfer& serial, PPU& ppu, CPU& cpu)
    : cartridge_(std::make_shared<Cartridge>())
    , ic_(ic)
    , joypad_(joypad)
    , timer_(timer)
    , serial_(serial)
    , ppu_(ppu)
    , cpu_(cpu)
    , wroteToSram_(false) {
}

void Memory::loadCartridge(std::shared_ptr<Cartridge> cartridge) {
    cartridge_ = cartridge;
}

u8 Memory::readVram(u8 bank, u16 addr) const {
    if ( addr < 0x8000 || addr > 0x9FFF )
        return 0xFF;
    return vram_.read(bank, addr - 0x8000);
}

void Memory::writeVram(u8 byte, u8 bank, u16 addr) {
    if (addr < 0x8000 || addr > 0x9FFF)
        return;
    vram_.write(byte, bank, addr - 0x8000);
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
        if(not ppu_.isEnabled() || ppu_.getMode() != 3)
            writeVram(byte, 0, addr - 0x8000);
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
        if(not ppu_.isEnabled() || ppu_.getMode() < 2)
            oam_[addr - 0xFE00] = byte;
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
            switch(addr) {
                case 0xFF04:
                    timer_.setDIV(0);
                    break;
                case 0xFF05:
                    timer_.setTIMA(byte);
                    break;
                case 0xFF06:
                    timer_.setTMA(byte);
                    break;
                case 0xFF07:
                    timer_.setTAC(byte);
                    break;
            }
        }
        // Interrupts
        else if (0xFF0F == addr) {
            ic_.setIF(byte);
        }
        // Audio - obsolete
        else if (0xFF10 <= addr && addr <= 0xFF26) {
        }
        // Wave pattern - obsolote
        else if (0xFF30 <= addr && addr <= 0xFF3F) {
        }
        // LCD - Control, Status, Position, Scrolling, Palettes
        else if (addr != 0xFF46 && 0xFF40 <= addr && addr <= 0xFF4B) {
            ppu_.write(byte, addr);
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
        ic_.setIE(byte);
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
        if(not ppu_.isEnabled() || ppu_.getMode() != 3)
            res = readVram(0, addr);
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
        /*if(not ppu_.isEnabled() || ppu_.getMode() < 2)*/
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
            switch(addr) {
                case 0xFF04:
                    return timer_.getDIV();
                case 0xFF05:
                    return timer_.getTIMA();
                case 0xFF06:
                    return timer_.getTMA();
                case 0xFF07:
                    return timer_.getTAC();
            }
        }
        // InterruptController
        else if (addr == 0xFF0F) {
            return ic_.getIF();
        }
        // PPU
        else if(addr != 0xFF46 && 0xFF40 <= addr && addr <= 0xFF4B) {
            res = ppu_.read(addr);
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
        return ic_.getIE();
    }
    return res;
}

void Memory::oamDmaTransfer(u8 byte) {
    if(byte > 0xF1)
        Utils::error("Attempted to write value outside 00-f1 in OAM DMA transfer!");

    static_assert(std::tuple_size<decltype(oam_)>::value == 0xA0, "OAM is incorrect size, should be 0xA0");

    for(u8 i = 0; i < 0x9F; ++i)
        oam_[i] = read(static_cast<u16>(byte << 8 | i));
    cpu_.addCycles(160 * 4);
}

void Memory::reset() {
    ic_.reset();
    timer_.reset();
    joypad_.reset();
    serial_.reset();
}
