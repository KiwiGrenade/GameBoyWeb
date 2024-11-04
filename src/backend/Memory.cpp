#include "Memory.hpp"
#include "Cartridge.hpp"
#include "utils.hpp"
#include <memory>

constexpr u16 Cartridge::romSize_;

Memory::Memory(InterruptController& ic, Joypad& joypad, Timer& timer)
    : memory_(std::array<unsigned char, size_>{})
    , cartridge_(std::make_shared<Cartridge>())
    , ic_(ic)
    , joypad_(joypad)
    , timer_(timer)
    , wroteToSram_(false) {
}

void Memory::loadCartridge(std::shared_ptr<Cartridge> cartridge) {
    std::cout << "hello!" << std::endl;
    cartridge_ = cartridge;
    std::cout << "world!" << std::endl;
}

void Memory::write(const u8 byte, const u16 addr) {
    if(isROM0(addr) || isROM1(addr) || isUndefined(addr)) {
        return;
        /*cartridge_.write(addr, byte);*/
        /*wroteToSram_ = false;*/
    }
    else if(isVRAM(addr)) {
        memory_[addr] = byte;
    }
    else if(isERAM(addr)) {
        memory_[addr] = byte;
    }
    else if(isWRAM0(addr)) {
        memory_[addr] = byte;
    }
    else if(isWRAM1(addr)) {
        memory_[addr] = byte;
    }
    else if(isECHO(addr)) {
        memory_[addr] = byte;
        memory_[addr-0x2000] = byte;
    }
    else if(isOAM(addr)) {
        memory_[addr] = byte;
    }
    else if(isIOPORT(addr)) {
        // joypad
        if(addr == 0xFF00) {
            joypad_.write(byte);
        }
        // serial transfer
        else if (0xFF01 <= addr && addr <= 0xFF02) {
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
        else if (0xFF40 <= addr && addr <= 0xFF4B) {
        }
        // set to non zero to disable Boot ROM
        else if (0xFF50 == addr) {
        }
        else {
            memory_[addr] = byte;
        }
    }
    else if(isHRAM(addr)) {
        memory_[addr] = byte;
    }
    else if (isIE(addr)) {
        ic_.setIE(byte);
    }
}

u8 Memory::read(const u16 addr) const {
    if(isROM0(addr) || isROM1(addr)) {
        return cartridge_->read(addr);
    }
    else if(isIOPORT(addr)) {
        if (addr == 0xFF00) {
            return joypad_.read();
        }
        if (0xFF04 <= addr && addr <= 0xFF07) {
            switch(addr) {
                case 0xFF04:
                    return timer_.getDIV();
                case 0xFF05:
                    return timer_.getTIMA();
                case 0xFF06:
                    return timer_.getTMA();
                case 0xFF07:
                    return timer_.getTAC();
                default:
                    return 0;
            }
        }
        else if (addr == 0xFF0F) {
            return ic_.getIF();
        }
        else
            return memory_[addr];
    }
    else if(isIE(addr)) {
        return ic_.getIE();
    }
    else {
        return memory_[addr];
    }
}

void Memory::reset() {
    memory_.fill(0);
    timer_.reset();
}
