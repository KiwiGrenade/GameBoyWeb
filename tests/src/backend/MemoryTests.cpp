#include "Memory.hpp"

#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators_adapters.hpp>
#include <catch2/generators/catch_generators_random.hpp>

struct MemoryTests : Memory {
    InterruptController ic;
    Timer timer = Timer(ic);
    Joypad joypad = Joypad(ic);
    MemoryTests() : Memory(ic, joypad, timer) {
    }
};

TEST_CASE_METHOD(MemoryTests, "write") {
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
        SECTION("IF") {
            write(byte, 0xFF0F);
            REQUIRE(ic_.getIF() == byte);
        }
        SECTION("TIMER") {
            SECTION("DIV") {
                write(byte, 0xFF04);
                REQUIRE(timer_.getDIV() == 0);
            }
            SECTION("DIV") {
                write(byte, 0xFF05);
                REQUIRE(timer_.getTIMA() == byte);
            }
            SECTION("DIV") {
                write(byte, 0xFF06);
                REQUIRE(timer_.getTMA() == byte);
            }
            SECTION("DIV") {
                write(byte, 0xFF07);
                REQUIRE(timer_.getTAC() == byte);
            }
        }
    }
    SECTION("HRAM") {
        u16 i = GENERATE(take(100, random(0xFF90, 0xFFFE)));

        write(byte, i);

        REQUIRE(memory_[i] == byte);
    }
    SECTION("IE") {
        write(byte, 0xFFFF);
        REQUIRE(ic_.getIE() == byte);
    }
};

TEST_CASE_METHOD(MemoryTests, "read") {
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
        SECTION("IF") {
            write(byte, 0xFFFF);
            REQUIRE(read(0xFFFF) == byte);
        }
        SECTION("TIMER") {
            SECTION("DIV") {
                write(byte, 0xFF04);
                REQUIRE(read(0xFF04) == 0);
            }
            SECTION("DIV") {
                write(byte, 0xFF05);
                REQUIRE(read(0xFF05) == byte);
            }
            SECTION("DIV") {
                write(byte, 0xFF06);
                REQUIRE(read(0xFF06) == byte);
            }
            SECTION("DIV") {
                write(byte, 0xFF07);
                REQUIRE(read(0xFF07) == byte);
            }
        }
    }
    SECTION("HRAM") {
        u16 i = GENERATE(take(100, random(0xFF90, 0xFFFE)));

        write(byte, i);

        REQUIRE(read(i) == byte);
    }
    SECTION("IE") {
        write(byte, 0xFFFF);
        REQUIRE(read(0xFFFF) == byte);
    }
};

TEST_CASE_METHOD(MemoryTests, "reset") {
    reset();
    for(u8 cell : memory_) {
        REQUIRE(cell == 0x0);
    }
}
