#include "Memory.hpp"

#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators_adapters.hpp>
#include <catch2/generators/catch_generators_random.hpp>

TEST_CASE_METHOD(Memory, "write") {
    u8 byte = GENERATE(take(1, random(1, 0xFF)));
    SECTION("ROM0") {
        u16 i = GENERATE(take(100, random(0, 0x3FFF)));

        write(byte, i);

        REQUIRE_FALSE(memory_[i] == byte);
    }
    SECTION("ROM1") {
        u16 i = GENERATE(take(100, random(0x4000, 0x7FFF)));

        write(byte, i);

        REQUIRE_FALSE(memory_[i] == byte);
    }
    SECTION("VRAM") {
        u16 i = GENERATE(take(100, random(0x8000, 0x9FFF)));

        write(byte, i);

        REQUIRE(memory_[i] == byte);
    }
    SECTION("ERAM") {
        u16 i = GENERATE(take(100, random(0xA000, 0xBFFF)));

        write(byte, i);

        REQUIRE(memory_[i] == byte);
    }
    SECTION("WRAM0") {
        u16 i = GENERATE(take(100, random(0xC000, 0xCFFF)));

        write(byte, i);

        REQUIRE(memory_[i] == byte);
    }
    SECTION("WRAM1") {
        u16 i = GENERATE(take(100, random(0xD000, 0xDFFF)));

        write(byte, i);

        REQUIRE(memory_[i] == byte);
    }
    SECTION("ECHO") {
        u16 i = GENERATE(take(100, random(0xE000, 0xFDFF)));

        write(byte, i);

        REQUIRE(memory_[i] == byte);
        REQUIRE(memory_[i-0x2000] == byte);
    }
    SECTION("0AM") {
        u16 i = GENERATE(take(100, random(0xFE00, 0xFE9F)));

        write(byte, i);

        REQUIRE(memory_[i] == byte);
    }
    SECTION("UNDEFINED") {
        u16 i = GENERATE(take(100, random(0xFEA0, 0xFEFF)));

        write(byte, i);

        REQUIRE_FALSE(memory_[i] == byte);
    }
    SECTION("IOPORT") {
        SECTION("JOYPAD") {
            SECTION("BUTTONS") {
                write(0b00010000, 0xFF00);
                REQUIRE(joypad_.inputDevice_ == Joypad::InputDevice::Buttons);
            }
            SECTION("PAD") {
                write(0b00100000, 0xFF00);
                REQUIRE(joypad_.inputDevice_ == Joypad::InputDevice::Pad);
            }
            SECTION("NOTHING") {
                write(0b00110000, 0xFF00);
                REQUIRE(joypad_.inputDevice_ == Joypad::InputDevice::Nothing);
            }
        }
        SECTION("TIMER") {
            constexpr u16 DIV = 0xFF04;
            constexpr u16 TIMA = 0xFF05;
            constexpr u16 TMA = 0xFF06;
            constexpr u16 TAC = 0xFF07;

            write(byte, DIV);
            REQUIRE(timer_.DIV_ == 0);

            write(byte, TIMA);
            REQUIRE(timer_.TIMA_  == byte);

            write(byte, TMA);
            REQUIRE(timer_.TMA_ == byte);

            write(byte, TAC);
            REQUIRE(timer_.TAC_ == byte);
        }
    }
    SECTION("HRAM") {
        u16 i = GENERATE(take(100, random(0xFF90, 0xFFFE)));

        write(byte, i);

        REQUIRE(memory_[i] == byte);
    }
    /*SECTION("IE") {*/
    /*    u16 i = 0xFFFF;*/
    /**/
    /*    write(byte, i);*/
    /**/
    /*    REQUIRE(memory_[i] == byte);*/
    /*}*/
};
TEST_CASE_METHOD(Memory, "read") {
    u8 byte = GENERATE(take(1, random(1, 0xFF)));
    SECTION("ROM0") {
        u16 i = GENERATE(take(100, random(0, 0x3FFF)));

        write(byte, i);

        REQUIRE_FALSE(read(i) == byte);
    }
    SECTION("ROM1") {
        u16 i = GENERATE(take(100, random(0x4000, 0x7FFF)));

        write(byte, i);

        REQUIRE_FALSE(read(i) == byte);
    }
    SECTION("VRAM") {
        u16 i = GENERATE(take(100, random(0x8000, 0x9FFF)));

        write(byte, i);

        REQUIRE(read(i) == byte);
    }
    SECTION("ERAM") {
        u16 i = GENERATE(take(100, random(0xA000, 0xBFFF)));

        write(byte, i);

        REQUIRE(read(i) == byte);
    }
    SECTION("WRAM0") {
        u16 i = GENERATE(take(100, random(0xC000, 0xCFFF)));

        write(byte, i);

        REQUIRE(read(i) == byte);
    }
    SECTION("WRAM1") {
        u16 i = GENERATE(take(100, random(0xD000, 0xDFFF)));

        write(byte, i);

        REQUIRE(read(i) == byte);
    }
    SECTION("ECHO") {
        u16 i = GENERATE(take(100, random(0xE000, 0xFDFF)));

        write(byte, i);

        REQUIRE(read(i) == byte);
        REQUIRE(read(i-0x2000) == byte);
    }
    SECTION("0AM") {
        u16 i = GENERATE(take(100, random(0xFE00, 0xFE9F)));

        write(byte, i);

        REQUIRE(read(i) == byte);
    }
    SECTION("UNDEFINED") {
        u16 i = GENERATE(take(100, random(0xFEA0, 0xFEFF)));

        REQUIRE(read(i) == 0x00);
    }
    SECTION("IOPORT") {
        SECTION("JOYPAD") {
            joypad_.buttons_ = 0b01110101;
            SECTION("BUTTONS") {
                write(0b00010000, 0xFF00);
                REQUIRE(read(0xFF00) == 0b11010101);
            }
            SECTION("PAD") {
                write(0b00100000, 0xFF00);
                REQUIRE(read(0xFF00) == 0b11100111);
            }
            SECTION("NOTHING") {
                write(0b00110000, 0xFF00);
                REQUIRE(read(0xFF00) == 0b11111111);
            }
        }
        SECTION("TIMER") {
            constexpr u16 DIV = 0xFF04;
            constexpr u16 TIMA = 0xFF05;
            constexpr u16 TMA = 0xFF06;
            constexpr u16 TAC = 0xFF07;

            timer_.DIV_ = byte;
            REQUIRE(read(DIV) == byte);

            timer_.TIMA_ = byte;
            REQUIRE(read(TIMA) == byte);

            timer_.TMA_ = byte;
            REQUIRE(read(TMA) == byte);

            timer_.TAC_ = byte;
            REQUIRE(read(TAC) == byte);
        }
    }
    SECTION("HRAM") {
        u16 i = GENERATE(take(100, random(0xFF90, 0xFFFE)));

        write(byte, i);

        REQUIRE(read(i) == byte);
    }
    /*SECTION("IE") {*/
    /*    u16 i = 0xFFFF;*/
    /**/
    /*    write(byte, i);*/
    /**/
    /*    REQUIRE(read(i) == byte);*/
    /*}*/
};

TEST_CASE_METHOD(Memory, "reset") {
    reset();
    for(u8 cell : memory_) {
        REQUIRE(cell == 0x0);
    }
}
