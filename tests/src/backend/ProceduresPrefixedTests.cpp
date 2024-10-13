#include "CPU.hpp"

#include <catch2/catch_test_macros.hpp>
#include <vector>

struct ProceduresPrefixedTests : CPU {
    ProceduresPrefixedTests() : CPU(memory) {
        AF_ = 0;
        BC_ = 0;
        DE_ = 0;
        HL_ = 0;
        SP_ = 0xFFFF;
        PC_ = 0;
    };
    Memory memory;

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
            /*std::cout << "i: " << i << std::endl << "k: " << k << std::endl;*/
            if(i == 6)
               k += jump;

            setUp(reg, i);
            execute(k);
            requirements(reg, i);
            
            k += jump;
            ++i;
        }
    }
};

TEST_CASE_METHOD(ProceduresPrefixedTests, "ProceduresPrefixedTests" ) {
    r16 oldAF = AF_;
    r16 oldBC = BC_;
    r16 oldDE = DE_;
    r16 oldHL = HL_;
    r16 oldSP = SP_;
    r16 oldPC = PC_;
    bool oldFlagZ = FlagZ_;
    bool oldFlagN = FlagN_;
    bool oldFlagH = FlagH_;
    bool oldFlagC = FlagC_;

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
        HL_ = 30;
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
        HL_ = 30;
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
        HL_ = 30;
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
        HL_ = 30;
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
        HL_ = 30;
        SECTION("shouldRLCorrectly") {
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
        HL_ = 30;
        SECTION("shouldRRCorrectly") {
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
}
