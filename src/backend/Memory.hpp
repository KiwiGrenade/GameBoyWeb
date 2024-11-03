#pragma once

#include <array>
#include "utils.hpp"

#include <QByteArray>
#include <memory>

#include "Cartridge.hpp"

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

class Memory {
public:
    Memory();
    ~Memory() = default;
    
    u8 read(const u16 addr) const;
    void write(const u8 byte, const u16 addr);
    void reset();
    inline void resetTimer() { timer_.reset(); };
    inline void setDIV(u8 val) { timer_.DIV_ = val; };
    inline u8& getIE() { return memory_[0xFFFF]; }
    inline u8& getIF() { return memory_[0xFF0F]; }
    void loadCartridge(std::shared_ptr<Cartridge>);

    static constexpr uint32_t size_ = 0x10000;

    struct Joypad {
        void reset() {
            inputDevice_ = InputDevice::Nothing;
            buttons_ = 0;
        };
        void write(u8 byte) {
            bool buttons = Utils::getBit(byte, 5);
            bool pad = Utils::getBit(byte, 4);
            // buttons are selected if 5-th bit is 0
            if(not buttons && pad) {
                inputDevice_ = InputDevice::Buttons;
            }
            else if(buttons && not pad) {
                // selected pad
                inputDevice_ = InputDevice::Pad;
            }
            else if(buttons && pad) {
                inputDevice_ = InputDevice::Nothing;
            }
            else {
                Utils::error("Both input devices selected!");
            }
        }

        u8 read() const {
            u8 hi = 0;
            u8 lo = 0;
            switch (inputDevice_) {
                case InputDevice::Buttons:
                    hi = 0b11010000;
                    lo = buttons_ & 0x0F;
                    return hi | lo;
                    break;
                case InputDevice::Pad:
                    hi = 0b11100000;
                    lo = (buttons_ & 0xF0) >> 4;
                    return hi | lo;
                    break;
                case InputDevice::Nothing:
                    return 0xFF;
                    break;
            }
        }

        u8 buttons_;

        enum class InputDevice {
            Buttons,
            Pad,
            Nothing
        };

        InputDevice inputDevice_;
    };

    Joypad joypad_;

protected:
    struct Timer {
        void reset() {
            DIV_ = 0;
            TIMA_ = 0;
            TMA_ = 0;
            TAC_ = 0;
        }
        u8 TIMA_ = 0;
        u8 TMA_ = 0;
        u8 TAC_ = 0;
        u16 DIV_ = 0;
    };
    
    Timer timer_;

    inline bool isROM0(const u16 addr) const { return 0 <= addr && addr <= 0x3FFF; };
    inline bool isROM1(const u16 addr) const { return 0x4000 <= addr && addr <= 0x7FFF; };
    inline bool isVRAM(const u16 addr) const { return 0x8000 <= addr && addr <= 0x9FFF; };
    inline bool isERAM(const u16 addr) const { return 0xA000 <= addr && addr <= 0xBFFF; };
    inline bool isWRAM0(const u16 addr) const { return 0xC000 <= addr && addr <= 0xCFFF; };
    inline bool isWRAM1(const u16 addr) const { return 0xD000 <= addr && addr <= 0xDFFF; };
    inline bool isECHO(const u16 addr) const { return 0xE000 <= addr && addr <= 0xFDFF; };
    inline bool isOAM(const u16 addr) const { return 0xFE00 <= addr && addr <= 0xFE9F; };
    inline bool isUndefined(const u16 addr) const { return 0xFEA0 <= addr && addr <= 0xFEFF; };
    inline bool isIOPORT(const u16 addr) const { return 0xFF00 <= addr && addr <= 0xFF7F; };
    inline bool isHRAM(const u16 addr) const { return 0xFF90 <= addr && addr <= 0xFFFE; };
    inline bool isIE(const u16 addr) const { return addr == 0xFFFF; };
    
    std::shared_ptr<Cartridge> cartridge_;
    std::array<u8, size_> memory_;
    bool wroteToSram_;
};
