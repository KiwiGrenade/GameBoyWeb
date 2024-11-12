#include "Memory.hpp"
#include "PPU.hpp"
#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators_adapters.hpp>
#include <catch2/generators/catch_generators_random.hpp>
#include <catch2/generators/catch_generators_range.hpp>
#include <cstdint>

TEST_CASE("memory.write/read") {
    extern Memory memory;
    extern CPU cpu;
    InterruptController ic;
    Joypad joypad { ic };
    Timer timer { ic };
    SerialDataTransfer serial { ic };
    PPU ppu { ic, memory, cpu};
    Memory memory { ic, timer, joypad, serial, ppu };
    CPU cpu { ic, memory };
    u8 byte = GENERATE(take(1, random(1, 0xFF)));

    SECTION("ROM0") {
        /*u16 i = GENERATE(take(100, random(0, 0x3FFF)));*/
        u16 i = GENERATE(take(100, random(0x0000, 0x3FFF)));

        memory.write(byte, i);

        REQUIRE_FALSE(memory.read(i) == byte);
    }
    SECTION("ROM1") {
        u16 i = GENERATE(take(100, random(0x4000, 0x7FFF)));

        memory.write(byte, i);

        REQUIRE_FALSE(memory.read(i) == byte);
    }
    SECTION("VRAM") {
        u16 i = GENERATE(take(100, random(0x8000, 0x9FFF)));

        memory.write(byte, i);

        REQUIRE(memory.read(i) == byte);
    }
    SECTION("ERAM") {
        u16 i = GENERATE(take(100, random(0xA000, 0xBFFF)));

        memory.write(byte, i);

        REQUIRE(memory.read(i) == byte);
    }
    SECTION("WRAM0") {
        u16 i = GENERATE(take(100, random(0xC000, 0xCFFF)));

        memory.write(byte, i);

        REQUIRE(memory.read(i) == byte);
    }
    SECTION("WRAM1") {
        u16 i = GENERATE(take(100, random(0xD000, 0xDFFF)));

        memory.write(byte, i);

        REQUIRE(memory.read(i) == byte);
    }
    SECTION("ECHO") {
        u16 i = GENERATE(take(100, random(0xE000, 0xFDFF)));

        memory.write(byte, i);

        REQUIRE(memory.read(i) == byte);
        REQUIRE(memory.read(i-0x2000) == byte);
    }
    SECTION("0AM") {
        u16 i = GENERATE(take(100, random(0xFE00, 0xFE9F)));

        memory.write(byte, i);

        REQUIRE(memory.read(i) == byte);
    }
    SECTION("UNDEFINED") {
        u16 i = GENERATE(take(100, random(0xFEA0, 0xFEFF)));

        memory.write(byte, i);

        REQUIRE_FALSE(memory.read(i) == byte);
    }
    SECTION("IOPORT") {
        SECTION("IF") {
            memory.write(byte, 0xFF0F);
            REQUIRE(ic.getIF() == byte);
        }
        SECTION("TIMER") {
            SECTION("DIV") {
                memory.write(byte, 0xFF04);
                REQUIRE(timer.getDIV() == 0);
            }
            SECTION("DIV") {
                memory.write(byte, 0xFF05);
                REQUIRE(timer.getTIMA() == byte);
            }
            SECTION("DIV") {
                memory.write(byte, 0xFF06);
                REQUIRE(timer.getTMA() == byte);
            }
            SECTION("DIV") {
                memory.write(byte, 0xFF07);
                REQUIRE(timer.getTAC() == byte);
            }
        }
    }
    SECTION("HRAM") {
        u16 i = GENERATE(take(100, random(0xFF90, 0xFFFE)));

        memory.write(byte, i);

        REQUIRE(memory.read(i) == byte);
    }
    SECTION("IE") {
        memory.write(byte, 0xFFFF);
        REQUIRE(ic.getIE() == byte);
    }
};

/*TEST_CASE_METHOD(MemoryTests, "read") {*/
/*    u8 byte = GENERATE(take(1, random(1, 0xFF)));*/
/*    SECTION("ROM0") {*/
/*        u16 i = GENERATE(take(100, random(0, 0x3FFF)));*/
/**/
/*        memory.write(byte, i);*/
/**/
/*        REQUIRE_FALSE(read(i) == byte);*/
/*    }*/
/*    SECTION("ROM1") {*/
/*        u16 i = GENERATE(take(100, random(0x4000, 0x7FFF)));*/
/**/
/*        memory.write(byte, i);*/
/**/
/*        REQUIRE_FALSE(read(i) == byte);*/
/*    }*/
/*    SECTION("VRAM") {*/
/*        u16 i = GENERATE(take(100, random(0x8000, 0x9FFF)));*/
/**/
/*        memory.write(byte, i);*/
/**/
/*        REQUIRE(read(i) == byte);*/
/*    }*/
/*    SECTION("ERAM") {*/
/*        u16 i = GENERATE(take(100, random(0xA000, 0xBFFF)));*/
/**/
/*        memory.write(byte, i);*/
/**/
/*        REQUIRE(read(i) == byte);*/
/*    }*/
/*    SECTION("WRAM0") {*/
/*        u16 i = GENERATE(take(100, random(0xC000, 0xCFFF)));*/
/**/
/*        memory.write(byte, i);*/
/**/
/*        REQUIRE(read(i) == byte);*/
/*    }*/
/*    SECTION("WRAM1") {*/
/*        u16 i = GENERATE(take(100, random(0xD000, 0xDFFF)));*/
/**/
/*        memory.write(byte, i);*/
/**/
/*        REQUIRE(read(i) == byte);*/
/*    }*/
/*    SECTION("ECHO") {*/
/*        u16 i = GENERATE(take(100, random(0xE000, 0xFDFF)));*/
/**/
/*        memory.write(byte, i);*/
/**/
/*        REQUIRE(read(i) == byte);*/
/*        REQUIRE(read(i-0x2000) == byte);*/
/*    }*/
/*    SECTION("0AM") {*/
/*        u16 i = GENERATE(take(100, random(0xFE00, 0xFE9F)));*/
/**/
/*        memory.write(byte, i);*/
/**/
/*        REQUIRE(read(i) == byte);*/
/*    }*/
/*    SECTION("UNDEFINED") {*/
/*        u16 i = GENERATE(take(100, random(0xFEA0, 0xFEFF)));*/
/**/
/*        REQUIRE(read(i) == 0x00);*/
/*    }*/
/*    SECTION("IOPORT") {*/
/*        SECTION("IF") {*/
/*            memory.write(byte, 0xFFFF);*/
/*            REQUIRE(read(0xFFFF) == byte);*/
/*        }*/
/*        SECTION("TIMER") {*/
/*            SECTION("DIV") {*/
/*                memory.write(byte, 0xFF04);*/
/*                REQUIRE(read(0xFF04) == 0);*/
/*            }*/
/*            SECTION("DIV") {*/
/*                memory.write(byte, 0xFF05);*/
/*                REQUIRE(read(0xFF05) == byte);*/
/*            }*/
/*            SECTION("DIV") {*/
/*                memory.write(byte, 0xFF06);*/
/*                REQUIRE(read(0xFF06) == byte);*/
/*            }*/
/*            SECTION("DIV") {*/
/*                memory.write(byte, 0xFF07);*/
/*                REQUIRE(read(0xFF07) == byte);*/
/*            }*/
/*        }*/
/*    }*/
/*    SECTION("HRAM") {*/
/*        u16 i = GENERATE(take(100, random(0xFF90, 0xFFFE)));*/
/**/
/*        memory.write(byte, i);*/
/**/
/*        REQUIRE(read(i) == byte);*/
/*    }*/
/*    SECTION("IE") {*/
/*        memory.write(byte, 0xFFFF);*/
/*        REQUIRE(read(0xFFFF) == byte);*/
/*    }*/
/*};*/

TEST_CASE("reset") {
    InterruptController ic;
    Joypad joypad { ic };
    Timer timer { ic };
    SerialDataTransfer serial { ic };
    PPU ppu { ic };
    Memory memory { ic, timer, joypad, serial, ppu };

    memory.reset();

    uint32_t i = 0;

    SECTION("0x0000 - 0xFF00") {
        i = GENERATE(range(0, 0xFF00));

        DYNAMIC_SECTION("Check if memory.read(" << std::hex << i << std::dec << ") == 0") {
            REQUIRE(memory.read(i) == 0);
        }
    }

    SECTION("0xFF00 - JOYPAD") {
        i = 0xFF00;
        REQUIRE(memory.read(i) == 0xFF);
    }

    SECTION("0xFF01 - 0xFF43") {
        i = GENERATE(range(0xFF01, 0xFF43));

        DYNAMIC_SECTION("Check if memory.read(" << std::hex << i << std::dec << ") == 0") {
            REQUIRE(memory.read(i) == 0);
        }
    }
    // lcd - temorary always returns 0x90
    SECTION("0xFF44") {
        i = 0xFF44;
        REQUIRE(memory.read(i) == 0x90);
    }
    SECTION("0xFF45 - 0xFFFF") {
        i = GENERATE(range(0xFF45, 0xFFFF));

        DYNAMIC_SECTION("Check if memory.read(" << std::hex << i << std::dec << ") == 0") {
            REQUIRE(memory.read(i) == 0);
        }
    }
}
