#include "MemoryBankController.hpp"

#include "Rom.hpp"
#include "Ram.hpp"

Mbc1::Mbc1(Rom *rom, std::optional<ERam> *ram)
    : rom_ {rom}, ram_ {ram}
{}

uint8_t Mbc1::read(uint16_t addr) const
{
    uint8_t b {0xFF};
    if (addr < 0x4000)
    {
        b = rom_->read(0, addr);
    }
    else if (addr < 0x8000)
    {
        b = rom_->read(romBank_, addr - 0x4000);
    }
    else if (addr < 0xa000)
    {
        // nothing defined here
    }
    else if (addr < 0xc000)
    {
        if (ram_ && ramEnable_)
            b = ram_->value().read(ramBank_, addr - 0xa000);
        else
            b = 0xff;
    }
    return b;
}

void Mbc1::write(uint8_t b, uint16_t addr)
{
    if (addr < 0x2000)
    {
        // 00h = disable RAM, 0ah == enable RAM
        ramEnable_ = ((b & 0xa) == 0xa);
    }
    else if (addr < 0x4000)
    {
        romBank_ = (b & 0x1f); // set low 5 bits with b
        adjustRomBank(); // ROM bank can't be 00h, 20h, 40h or 60h
    }
    else if (addr < 0x6000)
    {
        if (ramModeSelect_)
        {
            // if in RAM banking mode, use b as the ram bank number
            ramBank_ = (b & 3); // only banks 00-03 are addressable
            // only ROM banks 00-1f can be selected (hi 3 bits cleared)
            romBank_ &= ~0xe0;
            adjustRomBank();
        }
        else
        {
            // if in ROM banking mode, use b as the hi 2 bits of the rom bank
            // number
            romBank_ &= ~(3 << 6); // clear hi 2 bits
            romBank_ &= (b & (3 << 6)); // set hi 2 bits
            adjustRomBank(); // ROM bank can't be 00h, 20h, 40h or 60h
            // only RAM bank 00 can be selected
            ramBank_ = 0;
        }
    }
    else if (addr < 0x8000)
    {
        ramModeSelect_ = b;
    }
    else if (addr >= 0xa000 && addr < 0xc000)
    {
        if (ram_ && ramEnable_)
            ram_->value().write(b, ramBank_, addr - 0xa000);
    }
}

void Mbc1::adjustRomBank()
{
    // ROM banks 00, 20h, 40h, and 60h are not accessible. They convert
    // to 01h, 21h, 41h, and 61h instead
    switch (romBank_)
    {
        case 0x00:
        case 0x20:
        case 0x40:
        case 0x60:
            ++romBank_;
    }
}
