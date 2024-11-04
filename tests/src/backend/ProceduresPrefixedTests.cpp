#include "CPU.hpp"

#include <catch2/catch_test_macros.hpp>
#include <vector>

struct ProceduresPrefixedTests : CPU {
    InterruptController ic;
    Joypad jp = Joypad(ic);
    Memory memory = Memory(jp);

    ProceduresPrefixedTests() : CPU(memory) {
        AF_ = 0;
        BC_ = 0;
        DE_ = 0;
        HL_ = 0;
        SP_ = 0xFFFF;
        PC_ = 0xC000;
        E_ = 0;
    };

    void execute(u8 op) {
        memory_.write(0xCB, PC_);
        step();
        memory_.write(op, PC_);
        step();
    }

    void checkOnRegisters(const u8 startOpcode, const u8 jump, std::function<void(r8*, unsigned int)> setUp, std::function<void(r8*, unsigned int)>requirements) {
        std::vector<r8*> regs{&B_, &C_, &D_, &E_, &H_, &L_, &A_};
        unsigned int k = startOpcode;
        unsigned int i = 0;
        for(r8* reg : regs) {
            if(i == 6)
               k += jump;
            SECTION("forLoop") {
                setUp(reg, i);
                execute(k);
                requirements(reg, i);
            }
            
            k += jump;
            ++i;
        }
    }
};

TEST_CASE_METHOD(ProceduresPrefixedTests, "ProceduresPrefixedTests" ) {
    u8 constexpr exam = 0b11100101;

    SECTION("0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x07", "[RLC]") {
        checkOnRegisters(0x00, 1,
            [this] (r8* reg, unsigned int i) {
                FlagC_.set(false);
                *reg = 0b10001001;
            },
            [this] (r8* reg, unsigned int i) {
                REQUIRE(*reg == 0b00010011);
                REQUIRE(FlagC_);
            }
        );
    }
    SECTION("0x06", "[RLCHL]") {
        HL_ = 0xC300;
        SECTION("shouldRLCCorrectly") {
            memory_.write(0b10001001, HL_);
            execute(0x06);
            REQUIRE(fetch8(HL_) == 0b00010011);
        }
        SECTION("shouldSetCarryFlag") {
            memory_.write(0b10001001, HL_);
            execute(0x06);
            REQUIRE(FlagC_);
        }
        SECTION("shouldSetZeroFlag") {
            memory_.write(0, HL_);
            execute(0x06);
            REQUIRE(FlagZ_);
        }
    }
    SECTION("0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0F", "[RRC]") {
        checkOnRegisters(0x08, 1,
            [this] (r8* reg, unsigned int i) {
                FlagC_.set(false);
                *reg = 0b10001001;
            },
            [this] (r8* reg, unsigned int i) {
                REQUIRE(*reg == 0b11000100);
                REQUIRE(FlagC_);
            }
        );
    }
    SECTION("0x0E", "[RRCHL]") {
        HL_ = 0xC300;
        SECTION("shouldRRCCorrectly") {
            memory_.write(0b10001001, HL_);
            execute(0x0E);
            REQUIRE(fetch8(HL_) == 0b11000100);
        }
        SECTION("shouldSetCarryFlag") {
            memory_.write(0b10001001, HL_);
            execute(0x0E);
            REQUIRE(FlagC_);
        }
        SECTION("shouldSetZeroFlag") {
            memory_.write(0, HL_);
            execute(0x0E);
            REQUIRE(FlagZ_);
        }
    }
    SECTION("0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x17", "[RL]") {
        checkOnRegisters(0x10, 1,
            [this] (r8* reg, unsigned int i) {
                FlagC_.set(false);
                *reg = 0b10001001;
            },
            [this] (r8* reg, unsigned int i) {
                REQUIRE(*reg == 0b00010010);
                REQUIRE(FlagC_);
            }
        );
    }
    SECTION("0x16", "[RLHL]") {
        HL_ = 0xC300;
        SECTION("shouldRLCorrectly") {
            memory_.write(0b10001001, HL_);
            execute(0x16);
            REQUIRE(fetch8(HL_) == 0b00010010);
        }
        SECTION("shouldSetCarryFlag") {
            memory_.write(0b10001001, HL_);
            execute(0x16);
            REQUIRE(FlagC_);
        }
        SECTION("shouldSetZeroFlag") {
            memory_.write(0b10000000, HL_);
            execute(0x16);
            REQUIRE(FlagZ_);
        }
    }
    SECTION("0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1F", "[RR]") {
        checkOnRegisters(0x18, 1,
            [this] (r8* reg, unsigned int i) {
                FlagC_.set(false);
                *reg = 0b10001001;
            },
            [this] (r8* reg, unsigned int i) {
                REQUIRE(*reg == 0b01000100);
                REQUIRE(FlagC_);
            }
        );
    }
    SECTION("0x1E", "[RRHL]") {
        HL_ = 0xC300;
        SECTION("shouldRRCorrectly") {
            memory_.write(0b10001001, HL_);
            execute(0x1E);
            REQUIRE(fetch8(HL_) == 0b01000100);
        }
        SECTION("shouldSetCarryFlag") {
            memory_.write(0b10001001, HL_);
            execute(0x1E);
            REQUIRE(FlagC_);
        }
        SECTION("shouldSetZeroFlag") {
            memory_.write(0b00000001, HL_);
            execute(0x1E);
            REQUIRE(FlagZ_);
        }
    }
    SECTION("0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x27", "[SLA]") {
        checkOnRegisters(0x20, 1,
            [this] (r8* reg, unsigned int i) {
                *reg = 0b10001001;
            },
            [this] (r8* reg, unsigned int i) {
                REQUIRE(*reg == 0b00010011);
                REQUIRE(FlagC_);
            }
        );
    }
    SECTION("0x26", "[SLAHL]") {
        HL_ = 0xC300;
        SECTION("shouldSLACorrectly") {
            memory_.write(0b10001001, HL_);
            execute(0x26);
            REQUIRE(fetch8(HL_) == 0b00010011);
        }
        SECTION("shouldSetCarryFlag") {
            memory_.write(0b10001001, HL_);
            execute(0x26);
            REQUIRE(FlagC_);
        }
        SECTION("shouldSetZeroFlag") {
            memory_.write(0b10000000, HL_);
            execute(0x26);
            REQUIRE(FlagZ_);
        }
    }
    SECTION("0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2F", "[SRA]") {
        checkOnRegisters(0x28, 1,
            [this] (r8* reg, unsigned int i) {
                *reg = 0b10001001;
            },
            [this] (r8* reg, unsigned int i) {
                REQUIRE(*reg == 0b11000100);
                REQUIRE(FlagC_);
            }
        );
    }
    SECTION("0x2E", "[SRAHL]") {
        HL_ = 0xC300;
        SECTION("shouldSRACorrectly") {
            memory_.write(0b10001001, HL_);
            execute(0x2E);
            REQUIRE(fetch8(HL_) == 0b11000100);
        }
        SECTION("shouldSetCarryFlag") {
            memory_.write(0b10001001, HL_);
            execute(0x2E);
            REQUIRE(FlagC_);
        }
        SECTION("shouldSetZeroFlag") {
            memory_.write(0b00000001, HL_);
            execute(0x2E);
            REQUIRE(FlagZ_);
        }
    }
    SECTION("0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x37", "[SWAP]") {
        checkOnRegisters(0x30, 1,
            [this] (r8* reg, unsigned int i) {
                *reg = 0b11100101;
            },
            [this] (r8* reg, unsigned int i) {
                REQUIRE(*reg == 0b01011110);
            }
        );
    }
    SECTION("0x36", "[SWAPHL]") {
        HL_ = 0xC300;
        SECTION("shouldSwapCorrectly") {
            memory_.write(0b11100101, HL_);
            execute(0x36);
            REQUIRE(fetch8(HL_) == 0b01011110);
        }
        SECTION("shouldSetZeroFlag") {
            memory_.write(0b00000000, HL_);
            execute(0x36);
            REQUIRE(FlagZ_);
        }
    }
    SECTION("0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3F", "[SRL]") {
        checkOnRegisters(0x38, 1,
            [this] (r8* reg, unsigned int i) {
                *reg = 0b10001001;
            },
            [this] (r8* reg, unsigned int i) {
                REQUIRE(*reg == 0b01000100);
                REQUIRE(FlagC_);
            }
        );
    }
    SECTION("0x3E", "[SRAHL]") {
        HL_ = 0xC300;
        SECTION("shouldRRCorrectly") {
            memory_.write(0b10001001, HL_);
            execute(0x3E);
            REQUIRE(fetch8(HL_) == 0b01000100);
        }
        SECTION("shouldSetCarryFlag") {
            memory_.write(0b10001001, HL_);
            execute(0x3E);
            REQUIRE(FlagC_);
        }
        SECTION("shouldSetZeroFlag") {
            memory_.write(0b00000001, HL_);
            execute(0x3E);
            REQUIRE(FlagZ_);
        }
    }
// exam = 0b10100111
    SECTION("0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x47", "[BIT0]") {
        checkOnRegisters(0x40, 1,
            [this] (r8* reg, unsigned int i) {
                *reg = exam;
            },
            [this] (r8* reg, unsigned int i) {
                REQUIRE_FALSE(FlagZ_);
            }
        );
    }
    SECTION("0x48, 0x49, 0x4A, 0x4B, 0x4C, 0x4D, 0x4F", "[BIT1]") {
        checkOnRegisters(0x48, 1,
            [this] (r8* reg, unsigned int i) {
                *reg = exam;
            },
            [this] (r8* reg, unsigned int i) {
                REQUIRE(FlagZ_);
            }
        );
    }
    SECTION("0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x57", "[BIT2]") {
        checkOnRegisters(0x50, 1,
            [this] (r8* reg, unsigned int i) {
                FlagZ_.set(true);
                *reg = exam;
            },
            [this] (r8* reg, unsigned int i) {
                REQUIRE_FALSE(FlagZ_);
            }
        );
    }
    SECTION("0x58, 0x59, 0x5A, 0x5B, 0x5C, 0x5D, 0x5F", "[BIT3]") {
        checkOnRegisters(0x58, 1,
            [this] (r8* reg, unsigned int i) {
                FlagZ_.set(false);
                *reg = exam;
            },
            [this] (r8* reg, unsigned int i) {
                REQUIRE(FlagZ_);
            }
        );
    }
    SECTION("0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x67", "[BIT4]") {
        checkOnRegisters(0x60, 1,
            [this] (r8* reg, unsigned int i) {
                FlagZ_.set(false);
                *reg = exam;
            },
            [this] (r8* reg, unsigned int i) {
                REQUIRE(FlagZ_);
            }
        );
    }
    SECTION("0x68, 0x69, 0x6A, 0x6B, 0x6C, 0x6D, 0x6F", "[BIT5]") {
        checkOnRegisters(0x68, 1,
            [this] (r8* reg, unsigned int i) {
                FlagZ_.set(true);
                *reg = exam;
            },
            [this] (r8* reg, unsigned int i) {
                REQUIRE_FALSE(FlagZ_);
            }
        );
    }
    SECTION("0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x77", "[BIT6]") {
        checkOnRegisters(0x70, 1,
            [this] (r8* reg, unsigned int i) {
                FlagZ_.set(true);
                *reg = exam;
            },
            [this] (r8* reg, unsigned int i) {
                REQUIRE_FALSE(FlagZ_);
            }
        );
    }
    SECTION("0x78, 0x79, 0x7A, 0x7B, 0x7C, 0x7D, 0x7F", "[BIT7]") {
        checkOnRegisters(0x78, 1,
            [this] (r8* reg, unsigned int i) {
                FlagZ_.set(true);
                *reg = exam;
            },
            [this] (r8* reg, unsigned int i) {
                REQUIRE_FALSE(FlagZ_);
            }
        );
    }
    SECTION("0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x87", "[RES0]") {
        checkOnRegisters(0x80, 1,
            [this] (r8* reg, unsigned int i) {
                *reg = 0xFF;
            },
            [this] (r8* reg, unsigned int i) {
                REQUIRE_FALSE(Utils::getBit(*reg, 0));
            }
        );
    }
    SECTION("0x88, 0x89, 0x8A, 0x8B, 0x8C, 0x8D, 0x8F", "[RES1]") {
        checkOnRegisters(0x88, 1,
            [this] (r8* reg, unsigned int i) {
                *reg = 0xFF;
            },
            [this] (r8* reg, unsigned int i) {
                REQUIRE_FALSE(Utils::getBit(*reg, 1));
            }
        );
    }
    SECTION("0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x97", "[RES2]") {
        checkOnRegisters(0x90, 1,
            [this] (r8* reg, unsigned int i) {
                *reg = 0xFF;
            },
            [this] (r8* reg, unsigned int i) {
                REQUIRE_FALSE(Utils::getBit(*reg, 2));
            }
        );
    }
    SECTION("0x98, 0x99, 0x9A, 0x9B, 0x9C, 0x9D, 0x97", "[RES3]") {
        checkOnRegisters(0x98, 1,
            [this] (r8* reg, unsigned int i) {
                *reg = 0xFF;
            },
            [this] (r8* reg, unsigned int i) {
                REQUIRE_FALSE(Utils::getBit(*reg, 3));
            }
        );
    }
    SECTION("0xA0, 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA7", "[RES4]") {
        checkOnRegisters(0xA0, 1,
            [this] (r8* reg, unsigned int i) {
                *reg = 0xFF;
            },
            [this] (r8* reg, unsigned int i) {
                REQUIRE_FALSE(Utils::getBit(*reg, 4));
            }
        );
    }
    SECTION("0xA8, 0xA9, 0xAA, 0xAB, 0xAC, 0xAD, 0xAF", "[RES5]") {
        checkOnRegisters(0xA8, 1,
            [this] (r8* reg, unsigned int i) {
                *reg = 0xFF;
            },
            [this] (r8* reg, unsigned int i) {
                REQUIRE_FALSE(Utils::getBit(*reg, 5));
            }
        );
    }
    SECTION("0xB0, 0xB1, 0xB2, 0xB3, 0xB4, 0xB5, 0xB7", "[RES6]") {
        checkOnRegisters(0xB0, 1,
            [this] (r8* reg, unsigned int i) {
                *reg = 0xFF;
            },
            [this] (r8* reg, unsigned int i) {
                REQUIRE_FALSE(Utils::getBit(*reg, 6));
            }
        );
    }
    SECTION("0xB8, 0xB9, 0xBA, 0xBB, 0xBC, 0xBD, 0xBF", "[RES7]") {
        checkOnRegisters(0xB8, 1,
            [this] (r8* reg, unsigned int i) {
                *reg = 0xFF;
            },
            [this] (r8* reg, unsigned int i) {
                REQUIRE_FALSE(Utils::getBit(*reg, 7));
            }
        );
    }
    SECTION("0xC0, 0xC1, 0xC2, 0xC3, 0xC4, 0xC5, 0xC7", "[SET0]") {
        checkOnRegisters(0xC0, 1,
            [this] (r8* reg, unsigned int i) {
                *reg = 0x00;
            },
            [this] (r8* reg, unsigned int i) {
                REQUIRE(Utils::getBit(*reg, 0));
            }
        );
    }
    SECTION("0xC8, 0xC9, 0xCA, 0xCB, 0xCC, 0xCD, 0xCF", "[SET1]") {
        checkOnRegisters(0xC8, 1,
            [this] (r8* reg, unsigned int i) {
                *reg = 0x00;
            },
            [this] (r8* reg, unsigned int i) {
                REQUIRE(Utils::getBit(*reg, 1));
            }
        );
    }
    SECTION("0xD0, 0xD1, 0xD2, 0xD3, 0xD4, 0xD5, 0xD7", "[SET2]") {
        checkOnRegisters(0xD0, 1,
            [this] (r8* reg, unsigned int i) {
                *reg = 0x00;
            },
            [this] (r8* reg, unsigned int i) {
                REQUIRE(Utils::getBit(*reg, 2));
            }
        );
    }
    SECTION("0xD8, 0xD9, 0xDA, 0xDB, 0xDC, 0xDD, 0xD7", "[SET3]") {
        checkOnRegisters(0xD8, 1,
            [this] (r8* reg, unsigned int i) {
                *reg = 0x00;
            },
            [this] (r8* reg, unsigned int i) {
                REQUIRE(Utils::getBit(*reg, 3));
            }
        );
    }
    SECTION("0xE0, 0xE1, 0xE2, 0xE3, 0xE4, 0xE5, 0xE7", "[SET4]") {
        checkOnRegisters(0xE0, 1,
            [this] (r8* reg, unsigned int i) {
                *reg = 0x00;
            },
            [this] (r8* reg, unsigned int i) {
                REQUIRE(Utils::getBit(*reg, 4));
            }
        );
    }
    SECTION("0xE8, 0xE9, 0xEA, 0xEB, 0xEC, 0xED, 0xEF", "[SET5]") {
        checkOnRegisters(0xE8, 1,
            [this] (r8* reg, unsigned int i) {
                *reg = 0x00;
            },
            [this] (r8* reg, unsigned int i) {
                REQUIRE(Utils::getBit(*reg, 5));
            }
        );
    }
    SECTION("0xF0, 0xF1, 0xF2, 0xF3, 0xF4, 0xF5, 0xF7", "[SET6]") {
        checkOnRegisters(0xF0, 1,
            [this] (r8* reg, unsigned int i) {
                *reg = 0x00;
            },
            [this] (r8* reg, unsigned int i) {
                REQUIRE(Utils::getBit(*reg, 6));
            }
        );
    }
    SECTION("0xF8, 0xF9, 0xFA, 0xFB, 0xFC, 0xFD, 0xFF", "[SET7]") {
        checkOnRegisters(0xF8, 1,
            [this] (r8* reg, unsigned int i) {
                *reg = 0x00;
            },
            [this] (r8* reg, unsigned int i) {
                REQUIRE(Utils::getBit(*reg, 7));
            }
        );
    }
    // exam = 0b11100101
    SECTION("0x46, 0x4E, 0x56, 0x5E, 0x66, 0x6E, 0x76, 0x7E", "[BITHL]") {
        HL_ = 0xC300;
        memory_.write(exam, HL_);
        u8 k = 0x46;
        execute(k);
        REQUIRE_FALSE(FlagZ_);
        k+=8;
        execute(k);
        REQUIRE(FlagZ_);
        k+=8;
        execute(k);
        REQUIRE_FALSE(FlagZ_);
        k+=8;
        execute(k);
        REQUIRE(FlagZ_);
        k+=8;
        execute(k);
        REQUIRE(FlagZ_);
        k+=8;
        execute(k);
        REQUIRE_FALSE(FlagZ_);
        k+=8;
        execute(k);
        REQUIRE_FALSE(FlagZ_);
        k+=8;
        execute(k);
        REQUIRE_FALSE(FlagZ_);
    }
    SECTION("0x86, 0x8E, 0x86, 0x8E, 0x86, 0x8E, 0x86, 0x8E", "[RESHL]") {
        HL_ = 0xC300;
        memory_.write(0xFF, HL_);
        u8 k = 0x86;
        for(unsigned int i = 0; i < 8; ++i, k+=8) {
            execute(k);
            REQUIRE_FALSE(Utils::getBit(fetch8(HL_), i));
        }
        REQUIRE(fetch8(HL_) == 0);
    }
    SECTION("0xC6, 0xCE, 0xD6, 0xDE, 0xE6, 0xEE, 0xF6, 0xFE", "[SETHL]") {
        HL_ = 0xC300;
        memory_.write(0, HL_);
        u8 k = 0xC6;
        for(unsigned int i = 0; i < 8; ++i, k+=8) {
            execute(k);
            REQUIRE(Utils::getBit(fetch8(HL_), i));
        }
        REQUIRE(fetch8(HL_) == 0xFF);
    }
}
