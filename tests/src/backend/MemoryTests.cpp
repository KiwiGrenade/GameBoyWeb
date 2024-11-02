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
        /*u16 i = GENERATE(take(100, random(0xFF00, 0xFF7F)));*/
        /**/
        /*write(byte, i);*/
        /**/
        /*REQUIRE(memory_[i] == byte);*/
    }
    SECTION("HRAM") {
        u16 i = GENERATE(take(100, random(0xFF90, 0xFFFE)));

        write(byte, i);

        REQUIRE(memory_[i] == byte);
    }
    SECTION("IE") {
        u16 i = 0xFFFF;

        write(byte, i);

        REQUIRE(memory_[i] == byte);
    }
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
        /*u16 i = GENERATE(take(100, random(0xFF00, 0xFF7F)));*/
        /**/
        /*write(byte, i);*/
        /**/
        /*REQUIRE(read(i) == byte);*/
    }
    SECTION("HRAM") {
        u16 i = GENERATE(take(100, random(0xFF90, 0xFFFE)));

        write(byte, i);

        REQUIRE(read(i) == byte);
    }
    SECTION("IE") {
        u16 i = 0xFFFF;

        write(byte, i);

        REQUIRE(read(i) == byte);
    }
};

TEST_CASE_METHOD(Memory, "getIE") {
    u8 byte = GENERATE(take(1, random(0, 0xFF)));
    u8 byte1 = GENERATE(take(1, random(0, 0xFF)));
    u16 addr = 0xFFFF; // IE address

    u8& IE_ = getIE();

    REQUIRE(IE_ == 0);
    REQUIRE(memory_[addr] == 0);

    IE_ = byte;

    REQUIRE(IE_ == byte);
    REQUIRE(memory_[addr] == byte);

    memory_[addr] = byte1;

    REQUIRE(IE_ == byte1);
    REQUIRE(memory_[addr] == byte1);
}

TEST_CASE_METHOD(Memory, "getIF") {
    u8 byte = GENERATE(take(1, random(0, 0xFF)));
    u8 byte1 = GENERATE(take(1, random(0, 0xFF)));
    u16 addr = 0xFF0F; // IE address

    u8& IF_ = getIF();

    REQUIRE(IF_ == 0);
    REQUIRE(memory_[addr] == 0);

    IF_ = byte;

    REQUIRE(IF_ == byte);
    REQUIRE(memory_[addr] == byte);

    memory_[addr] = byte1;

    REQUIRE(IF_ == byte1);
    REQUIRE(memory_[addr] == byte1);
}

TEST_CASE_METHOD(Memory, "reset") {
    reset();
    for(u8 cell : memory_) {
        REQUIRE(cell == 0x0);
    }
}
