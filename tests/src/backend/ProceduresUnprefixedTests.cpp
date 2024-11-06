#include "CPU.hpp"

#include <catch2/catch_test_macros.hpp>
#include <vector>

struct ProceduresUnprefixedTests : CPU {
    InterruptController ic;
    Joypad jp = Joypad(ic);
    Timer timer = Timer(ic);
    Memory memory = Memory(ic, jp, timer);

    ProceduresUnprefixedTests() : CPU(ic, memory) {
        F_ = 0;
        HL_ = 0xCC00;
        PC_ = 0xC000;
    };

    u8 execute(u8 op) {
        memory_.write(op, PC_);
        return step();
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

TEST_CASE_METHOD(ProceduresUnprefixedTests, "ProceduresUnprefixedTests" ) {
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

    SECTION("0x01, 0x11, 0x21, 0x31", "[LD16]") {
        std::vector<r16*> regPairs {&BC_, &DE_, &HL_, &SP_};
        u8 k = 0x01;
        for(u8 i = 0; i < regPairs.size(); i++, k+=16) {
            memory_.write(0b00110011, PC_+1);
            memory_.write(0b11000011, PC_+2);
            execute(k);
            REQUIRE(*regPairs[i] == 0b1100001100110011);
        }
    }
    SECTION("0x02", "[LDD]") {
        BC_ = 0xC011;
        A_ = 12;
        execute(0x02);
        REQUIRE(fetch8(BC_) == A_);
    }
    SECTION("0x03, 0x13, 0x23, 0x33", "[INC]") {
        std::vector<r16*> regPairs {&BC_, &DE_, &HL_, &SP_};
        u8 k = 0x03;
        for(u16 i = 0; i < regPairs.size(); i++, k+=16) {
            *regPairs[i] = i;
            execute(k);
            REQUIRE(regPairs[i]->getVal() - 1 == i);
        }
    }
    SECTION("0x04, 0x0C, 0x14, 0x1C, 0x24, 0x2C, 0x3C", "[INC]") {
        checkOnRegisters(0x04, 8,
            [this] (r8* reg, unsigned int i) {
                *reg = 20;
            },
            [this] (r8* reg, unsigned int i) {
                REQUIRE(*reg == 21);
            }
        );
        SECTION("shouldSetHalfCarryFlag") {
            B_ = 0x0F;
            execute(0x04);
            REQUIRE(FlagH_);
        }
        SECTION("shouldSetZeroFlag") {
            B_ = 0xFF;
            execute(0x04);
            REQUIRE(FlagZ_);
        }
    }
    SECTION("0x05, 0x0D, 0x15, 0x1D, 0x25, 0x2D, 0x3D", "[DEC]") {
        checkOnRegisters(0x05, 8,
            [this] (r8* reg, unsigned int i) {
                *reg = 20;
            },
            [this] (r8* reg, unsigned int i) {
                REQUIRE(*reg == 19);
            }
        );
        SECTION("shouldSetHalfCarryFlag") {
            B_ = 0;
            execute(0x05);
            REQUIRE(FlagH_);
        }
        SECTION("shouldSetZeroFlag") {
            B_ = 1;
            execute(0x05);
            REQUIRE(FlagZ_);
        }
    }
    SECTION("0x06, 0x0E, 0x16, 0x1E, 0x26, 0x2E, 0x3E", "[LD]") {
        checkOnRegisters(0x06, 8,
            [this] (r8* reg, unsigned int i) {
                memory_.write(30, PC_+1);
            },
            [this] (r8* reg, unsigned int i) {
                REQUIRE(*reg == 30);
            }
        );
    }
    SECTION("0x07", "[RLCA]") {
        A_ = 0b10000001;
        FlagC_.set(false);

        execute(0x07);
        REQUIRE(FlagC_);
        REQUIRE(A_ == 0b00000011);
    }
    SECTION("0x08", "[LDD16]") {
        SP_ = 28;
        memory_.write(0x90, PC_+1);
        memory_.write(0xC9, PC_+2);
        execute(0x08);
        REQUIRE(fetch8(0xC990) == SP_);
    }
    SECTION("0x09, 0x19, 0x29, 0x39", "[ADD16]") {
        std::vector<r16*> regPairs{&BC_, &DE_, &HL_, &SP_};
        u8 k = 0x09;
        for(u16 i = 0; i < regPairs.size(); i++, k+=16) {
            HL_ = 20;
            *regPairs[i] = 30;
            execute(k);
            if(i != 2)
                REQUIRE(HL_ == 50);
            else
                REQUIRE(HL_ == 60);
        }
        SECTION("shouldSetHalfCarryFlag") {
            HL_ = 0b0000100000000000;
            BC_ = 0b0000100000000000;
            execute(0x09);
            REQUIRE(FlagH_);
        }
        SECTION("shouldSetCarryFlag") {
            HL_ = 0xFFFF;
            BC_ = 0xFFFF;
            execute(0x09);
            REQUIRE(FlagC_);
        }
    }
    SECTION("0x0A", "[LD]") {
        BC_ = 0xC800;
        u8 val = 14;
        memory.write(val, BC_);
        execute(0x0A);
        REQUIRE(A_ == val);
    }
    SECTION("0x0B, 0x1B, 0x2B, 0x3B", "[DEC]") {
        std::vector<r16*> regPairs{&BC_, &DE_, &HL_, &SP_};
        u8 k = 0x0B;
        for(u16 i = 0; i < regPairs.size(); i++, k+=16) {
            *regPairs[i] = i;
            execute(k);
            REQUIRE(*regPairs[i] + 1 == i);
        }
    }
    SECTION("0x0F", "[RRCA]") {
        A_ = 0b10000001;
        FlagC_.set(false);
        
        execute(0x0F);
        REQUIRE(FlagC_);
        REQUIRE(A_ == 0b11000000);
    }
    SECTION("0x10", "[STOP]") {
        execute(0x10);
        REQUIRE(isStopped_);
    }
    SECTION("0x12", "[LDD]") {
        DE_ = 0xC900;
        A_ = 16;
        execute(0x12);
        REQUIRE(fetch8(DE_) == A_);
    }
    SECTION("0x17, [RLA]") {
        A_ = 0b11000000;
        FlagC_.set(false);
        
        execute(0x17);
        REQUIRE(FlagC_);
        REQUIRE(A_ == 0b10000000);

        A_ = 0b01000000;
        FlagC_.set(true);

        execute(0x17);
        REQUIRE_FALSE(FlagC_);
        REQUIRE(A_ == 0b10000001);
    }
    SECTION("0x18", "[JR]") {
        SECTION("positive") {
            memory_.write(int8_t(20), PC_+1);
            execute(0x18);
            REQUIRE(PC_ == oldPC+20);
        }
        SECTION("negative") {
            memory_.write(int8_t(-20), PC_+1);
            execute(0x18);
            REQUIRE(PC_ == oldPC.getVal()-20);
        }
    }
    SECTION("0x1A", "[LD]") {
        DE_ = 0xCC00;
        u8 val = 18;
        memory_.write(val, DE_);
        execute(0x1A);
        REQUIRE(A_ == val);
    }
    SECTION("0x1F", "[rra]") {
        A_ = 0b00000001;

        execute(0x1F);
        REQUIRE(FlagC_);
        REQUIRE(A_ == 0);
        
        execute(0x1F);
        REQUIRE_FALSE(FlagC_);
        REQUIRE(A_ == 0b10000000);
    }
    SECTION("0x20", "[JR]") {
        int8_t val = 20;
        u8 op = 0x20;
        SECTION("shouldJump") {
            memory_.write(int8_t(val), PC_+1);
            execute(op);
            REQUIRE(PC_ == oldPC+val);
        }
        SECTION("shouldNotJump") {
            FlagZ_.set(true);
            memory_.write(int8_t(val), PC_+1);
            execute(op);
            REQUIRE(PC_ == oldPC+2);
        }
    }
    SECTION("0x22", "[LDD]") {
        HL_ = 0xC017;
        u16 oldHL = HL_;
        A_ = 18;
        execute(0x22);
        REQUIRE(oldHL + 1 == HL_);
        REQUIRE(fetch8(oldHL) == A_);
    }
    SECTION("0x27", "[DAA]") {
        A_ = 0x9B;
        execute(0x27);
        REQUIRE(FlagC_);
        REQUIRE(A_ == 1);
    }
    SECTION("0x28", "[JR]") {
        int8_t val = 20;
        u8 op = 0x28;
        SECTION("shouldJump") {
            FlagZ_.set(true);
            memory_.write(int8_t(val), PC_+1);
            execute(op);
            REQUIRE(PC_ == oldPC+val);
        }
        SECTION("shouldNotJump") {
            memory_.write(int8_t(val), PC_+1);
            execute(op);
            REQUIRE(PC_ == oldPC+2);
        }
    }
    SECTION("0x2A", "[LD]") {
        HL_ = 0xCC19;
        u16 oldHL = HL_;
        u8 val = 20;
        memory.write(val, HL_);
        execute(0x2A);
        REQUIRE(oldHL + 1 == HL_);
        REQUIRE(A_ == val);
    }
    SECTION("0x2F", "[CPL]") {
        A_ = u8(0b11010100);
        execute(0x2f);
        REQUIRE(A_ == u8(0b00101011));
    }
    SECTION("0x30", "[JR]") {
        int8_t val = 20;
        u8 op = 0x30;
        SECTION("shouldJump") {
            memory_.write(int8_t(val), PC_+1);
            execute(op);
            REQUIRE(PC_ == oldPC+val);
        }
        SECTION("shouldNotJump") {
            FlagC_.set(true);
            memory_.write(int8_t(val), PC_+1);
            execute(op);
            REQUIRE(PC_ == oldPC+2);
        }
    }
    SECTION("0x32", "[LDD]") {
        HL_ = 0xC021;
        u16 oldHL = HL_;
        A_ = 22;
        execute(0x32);
        REQUIRE(oldHL - 1 == HL_);
        REQUIRE(fetch8(oldHL) == A_);
    }
    SECTION("0x34", "[INCD]") {
        // testing flags in 0x04
        u8 val = 21;
        HL_ = 0xFACB;
        memory_.write(val, HL_);
        execute(0x34);
        REQUIRE(fetch8(HL_) == val+1);
    }
    SECTION("0x35", "[DECD]") {
        // testing flags in 0x05
        u8 val = 21;
        HL_ = 0xFACB;
        memory_.write(val, HL_);
        execute(0x35);
        REQUIRE(fetch8(HL_) == val-1);
    }
    SECTION("0x36", "[LDD]") {
        HL_ = 0xCC10;
        u8 PC_plus1Val = 11;
        memory_.write(PC_plus1Val, PC_+1);
        execute(0x36);
        REQUIRE(fetch8(HL_) == PC_plus1Val);
    }
    SECTION("0x37", "[SCF]") {
        execute(0x37);
        REQUIRE_FALSE(FlagN_);
        REQUIRE_FALSE(FlagH_);
        REQUIRE(FlagC_);
    }
    SECTION("0x38", "[JR]") {
        int8_t val = 20;
        u8 op = 0x38;
        SECTION("shouldJump") {
            FlagC_.set(true);
            memory_.write(int8_t(val), PC_+1);
            execute(op);
            REQUIRE(PC_ == oldPC+val);
        }
        SECTION("shouldNotJump") {
            memory_.write(int8_t(val), PC_+1);
            execute(op);
            REQUIRE(PC_ == oldPC+2);
        }
    }
    SECTION("0x3A", "[LD]") {
        HL_ = 0xCC23;
        u16 oldHL = HL_;
        u8 val = 24;
        memory.write(val, HL_);
        execute(0x3A);
        REQUIRE(oldHL - 1 == HL_);
        REQUIRE(A_ == val);
    }
    SECTION("0x3F", "[CCF]") {
        execute(0x3f);
        REQUIRE(oldFlagC != FlagC_);
        execute(0x3f);
        REQUIRE(oldFlagC == FlagC_);
    }
    SECTION("0x40-0x7F", "[LD], [LDD], [HALT]") {
        SECTION("0x76", "[HALT]") {
            u16 IF = 0xFF0F;
            u16 IE = 0xFFFF;
            isHalted_ = false;
            IME_ = true;
            execute(0x76);
            REQUIRE(isHalted_);

            isHalted_ = false;
            IME_ = false;
            memory_.write(0, IE);
            memory_.write(1, IF);
            execute(0x76);
            REQUIRE(isHalted_);

            isHalted_ = false;
            IME_ = false;
            memory_.write(1, IE);
            memory_.write(1, IF);
            execute(0x76);
            REQUIRE(isHaltBug_);
        }
        std::vector<u8*> reg {&B_, &C_, &D_, &E_, &H_, &L_, &A_};
        u8 k;
        u8 op;
        SECTION("0x40-0x45, 0x47-0x4D, 0x4F-0x55, 0x57-0x5D, 0x5F-0x65, 0x67-0x6D, 0x6F, 0x78-0x7D, 0x7F", "[LD]") {
            k = 0x40;
            for(u8 i = 0; i < 8; i++, k += 8) {
                if(k == 0x70)
                    continue;
                for(u8 j = 0; j < 8; j++) {
                    if(j == 6) 
                        continue;
                    op = k + j;
                    u8* to = i == 7 ? reg[i-1] : reg[i];
                    u8* from = j == 7 ? reg[j-1] : reg[j];
                    *from = op;
                    execute(op);
                    REQUIRE(*to == *from);
                }
            }
        }
        SECTION("0x70-0x75, 0x77", "[LDD]") {
            checkOnRegisters(0x70, 1,
                [this] (r8* reg, unsigned int i) {
                    HL_ = 0xCF40;
                    *reg = 0xC0;
                },
                [this] (r8* reg, unsigned int i) {
                    if(i == 4)
                    REQUIRE(fetch8(HL_) == 0xC0);
                }
            );
        }
        SECTION("0x46, 0x4E, 0x56, 0x5E, 0x66, 0x6E, 0x7E") {
            checkOnRegisters(0x46, 8,
                [this] (r8* reg, unsigned int i) {
                    HL_ = 0xCC40;
                    memory_.write(20, HL_);
                },
                [this] (r8* reg, unsigned int i) {
                    REQUIRE(*reg == 20);
                }
            );
        }
    }
    SECTION("0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x87", "[ADD]") {
        checkOnRegisters(0x80, 1,
            [this] (r8* reg, unsigned int i) {
                A_ = 2;
                *reg = 0xFF;
            },
            [this] (r8* reg, unsigned int i) {
                if(i == 6)
                    REQUIRE(A_ == 0xFE);
                else
                    REQUIRE(A_ == 1);
            }
        );
    }
    SECTION("0x86", "[ADD]") {
        SECTION("shouldAddCorrectly") {
            A_ = 20;
            memory.write(20, HL_);
            execute(0x86);
            REQUIRE(A_ == 40);

            A_ = 2;
            memory.write(0xFF, HL_);
            execute(0x86);
            REQUIRE(A_ == 1);
        }
        SECTION("shouldSetHalfCarryFlag") {
            A_ = 0b00001000;
            memory.write(A_, HL_);
            execute(0x86);
            REQUIRE_FALSE(FlagZ_);
            REQUIRE(FlagH_);
            REQUIRE_FALSE(FlagC_);
        }
        SECTION("shouldSetHalfCarryFlagZeroFlagAndCarryFlag") {
            A_ = 0xFF;
            memory.write(1, HL_);
            execute(0x86);
            REQUIRE(FlagZ_);
            REQUIRE(FlagH_);
            REQUIRE(FlagC_);
        }
    }
    SECTION("0x88, 0x89, 0x8A, 0x8B, 0x8C, 0x8D, 0x8F", "[ADC]") {
        SECTION("FlagC_ = true") {
            checkOnRegisters(0x88, 1,
                [this] (r8* reg, unsigned int i) {
                    FlagC_.set(true);
                    A_ = 2;
                    *reg = 0xFF;
                },
                [this] (r8* reg, unsigned int i) {
                    if(i == 6)
                        REQUIRE(A_ == 0xFF);
                    else
                        REQUIRE(A_ == 2);
                }
            );
        }
        SECTION("FlagC_ = false") {
            checkOnRegisters(0x88, 1,
                [this] (r8* reg, unsigned int i) {
                    FlagC_.set(false);
                    A_ = 2;
                    *reg = 0xFF;
                },
                [this] (r8* reg, unsigned int i) {
                    if(i == 6)
                        REQUIRE(A_ == 0xFE);
                    else
                        REQUIRE(A_ == 1);
                }
            );
        }
    }
    SECTION("0x8E", "[ADC]") {
        SECTION("FlagC_ = false") {
            FlagC_.set(false);
            A_ = 20;
            memory.write(20, HL_);
            execute(0x8E);
            REQUIRE(A_ == 40);

            FlagC_.set(false);
            A_ = 2;
            memory.write(0xFF, HL_);
            execute(0x8E);
            REQUIRE(FlagC_);
            REQUIRE(A_ == 1);
        }
        SECTION("FlagC_ = true") {
            FlagC_.set(true);
            A_ = 20;
            memory.write(20, HL_);
            execute(0x8E);
            REQUIRE(A_ == 41);

            FlagC_.set(true);
            A_ = 2;
            memory.write(0xFF, HL_);
            execute(0x8E);
            REQUIRE(A_ == 2);
        }
        SECTION("shouldSetHalfCarryFlag") {
            A_ = 0b00001000;
            memory.write(A_, HL_);
            execute(0x8E);
            REQUIRE_FALSE(FlagZ_);
            REQUIRE(FlagH_);
            REQUIRE_FALSE(FlagC_);
        }
        SECTION("shouldSetHalfCarryFlagZeroFlagAndCarryFlag") {
            A_ = 0xFF;
            memory.write(1, HL_);
            execute(0x8E);
            REQUIRE(FlagZ_);
            REQUIRE(FlagH_);
            REQUIRE(FlagC_);
        }
    }
    SECTION("0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x97", "[SUB]") {
        checkOnRegisters(0x90, 1,
            [this] (r8* reg, unsigned int i) {
                A_ = 2;
                *reg = 0xFF;
            },
            [this] (r8* reg, unsigned int i) {
                if(i == 6)
                    REQUIRE(A_ == 0);
                else
                    REQUIRE(A_ == 3);
            }
        );
    }
    SECTION("0x96", "[SUB]") {
        SECTION("shouldSubstractCorrectly") {
            A_ = 20;
            memory.write(20, HL_);
            execute(0x96);
            REQUIRE(A_ == 0);

            A_ = 2;
            memory.write(0xFF, HL_);
            execute(0x96);
            REQUIRE(A_ == 3);
        }
        SECTION("shouldSetHalfCarryFlagAndCarryFlag") {
            A_ = 0;
            memory.write(1, HL_);
            execute(0x96);
            REQUIRE_FALSE(FlagZ_);
            REQUIRE(FlagH_);
            REQUIRE(FlagC_);
        }
        SECTION("shouldSetZeroFlag") {
            A_ = 0xFF;
            memory.write(A_, HL_);
            execute(0x96);
            REQUIRE(FlagZ_);
            REQUIRE_FALSE(FlagH_);
            REQUIRE_FALSE(FlagC_);
        }
    }
    SECTION("0x98, 0x99, 0x9A, 0x9B, 0x9C, 0x9D, 0x9F", "[SBC]") {
        SECTION("FlagC_ = true") {
            checkOnRegisters(0x98, 1,
                [this] (r8* reg, unsigned int i) {
                    FlagC_.set(true);
                    A_ = 2;
                    *reg = 0xFF;
                },
                [this] (r8* reg, unsigned int i) {
                    if(i == 6)
                        REQUIRE(A_ == 0xFF);
                    else
                        REQUIRE(A_ == 2);
                }
            );
        }
        SECTION("FlagC_ = false") {
            checkOnRegisters(0x98, 1,
                [this] (r8* reg, unsigned int i) {
                    FlagC_.set(false);
                    A_ = 2;
                    *reg = 0xFF;
                },
                [this] (r8* reg, unsigned int i) {
                    if(i == 6)
                        REQUIRE(A_ == 0);
                    else
                        REQUIRE(A_ == 3);
                }
            );
        }
    }
    SECTION("0x9E", "[SBC]") {
        SECTION("shouldSubstractCorrectly") {
            A_ = 20;
            memory.write(20, HL_);
            execute(0x9E);
            REQUIRE(A_ == 0);

            FlagC_.set(true);
            A_ = 2;
            memory.write(0xFF, HL_);
            execute(0x9E);
            REQUIRE(A_ == 2);
        }
        SECTION("shouldSetHalfCarryFlagAndCarryFlag") {
            FlagC_.set(true);
            A_ = 0;
            memory.write(1, HL_);
            execute(0x9E);
            REQUIRE_FALSE(FlagZ_);
            REQUIRE(FlagH_);
            REQUIRE(FlagC_);
        }
        SECTION("shouldSetZeroFlag") {
            FlagC_.set(true);
            A_ = 0xFF;
            memory.write(A_-1, HL_);
            execute(0x9E);
            REQUIRE(FlagZ_);
            REQUIRE_FALSE(FlagH_);
            REQUIRE_FALSE(FlagC_);
        }
    }
    SECTION("0xA0, 0xA1, 0xA2, 0xA3, 0xA3, 0xA5, 0xA7", "[AND]") {
        checkOnRegisters(0xA0, 1,
            [this] (r8* reg, unsigned int i) {
                A_      = 0b11100101;
                *reg    = 0b11000111;
            },
            [this] (r8* reg, unsigned int i) {
                if(i == 6)
                    REQUIRE(A_ == 0b11000111);
                else
                    REQUIRE(A_ == 0b11000101);
            }
        );
    }
    SECTION("0xA6", "[AND]") {
        SECTION("shouldLogicANDCorrectly") {
            A_ =  0b11100101;
            memory.write(0b11000111, HL_);
            execute(0xA6);
            REQUIRE(A_ == 0b11000101);
        }
        SECTION("shouldSetZeroFlag") {
            A_ = 0b11100010;
            memory.write(0b00011101, HL_);
            execute(0xA6);
            REQUIRE(FlagZ_);
        }
    }
    SECTION("0xA8, 0xA9, 0xAA, 0xAB, 0xAC, 0xAD, 0xAF", "[XOR]") {
        checkOnRegisters(0xA8, 1,
            [this] (r8* reg, unsigned int i) {
                A_      = 0b11100101;
                *reg    = 0b11000111;
            },
            [this] (r8* reg, unsigned int i) {
                if(i == 6)
                    REQUIRE(A_ == 0b00000000);
                else 
                    REQUIRE(A_ == 0b00100010);
            }
        );
    }
    SECTION("0xAE", "[XOR]") {
        SECTION("shouldLogicORCorrectly") {
            A_ =  0b11100101;
            memory.write(0b11000111, HL_);
            u8 c = execute(0xAE);
            REQUIRE(A_ == 0b00100010);
            REQUIRE(c == 8);
        }
        SECTION("shouldSetZeroFlag") {
            A_ = 23;
            memory.write(23, HL_);
            execute(0xAE);
            REQUIRE(FlagZ_);
        }
    }
    SECTION("0xB0, 0xB1, 0xB2, 0xB3, 0xB4, 0xB5, 0xB7", "[OR]") {
        checkOnRegisters(0xB0, 1,
            [this] (r8* reg, unsigned int i) {
                A_      = 0b11100101;
                *reg    = 0b11000111;
            },
            [this] (r8* reg, unsigned int i) {
                if(i == 6)
                    REQUIRE(A_ == 0b11000111);
                else 
                    REQUIRE(A_ == 0b11100111);
            }
        );
    }
    SECTION("0xB6", "[OR]") {
        SECTION("shouldLogicXORCorrectly") {
            A_ =  0b11100101;
            memory.write(0b11000111, HL_);
            execute(0xB6);
            REQUIRE(A_ == 0b11100111);
        }
        SECTION("shouldSetZeroFlag") {
            A_ = 0;
            memory.write(0, HL_);
            execute(0xB6);
            REQUIRE(FlagZ_);
        }
    }
    SECTION("0xB8, 0xB9, 0xBA, 0xBB, 0xBC, 0xBD, 0xBF", "[CP]") {
        SECTION("shouldCPCorrectly") {
            checkOnRegisters(0xB8, 1,
                [this] (r8* reg, unsigned int i) {
                    A_      = 21;
                    *reg    = 23;
                },
                [this] (r8* reg, unsigned int i) {
                    if(i == 6)
                        REQUIRE(A_ == 23);
                    else 
                        REQUIRE(A_ == 21);
                }
            );
        }
        SECTION("shouldSetZeroFlag") {
            checkOnRegisters(0xB8, 1,
                [this] (r8* reg, unsigned int i) {
                    A_      = 28;
                    *reg    = 28;
                },
                [this] (r8* reg, unsigned int i) {
                    REQUIRE(FlagZ_);
                }
            );
        }
        SECTION("shouldSetHalfCarry") {
            checkOnRegisters(0xB8, 1,
                [this] (r8* reg, unsigned int i) {
                    A_      = 1;
                    *reg    = 0xFF;
                },
                [this] (r8* reg, unsigned int i) {
                    if(i != 6)
                        REQUIRE(FlagH_);
                }
            );
        }
    }
    SECTION("0xBE", "[CP]") {
        SECTION("shouldCPCorrectly") {
            A_ =  21;
            memory.write(23, HL_);
            execute(0xBE);
            REQUIRE(A_ == 21);
        }
        SECTION("shouldSetZeroFlag") {
            A_ =  28;
            memory.write(28, HL_);
            execute(0xBE);
            REQUIRE(FlagZ_);
        }
        SECTION("shouldSetHalfCarry") {
            A_ =  1;
            memory.write(0xFF, HL_);
            execute(0xBE);
            REQUIRE(FlagH_);
        }
    }
    SECTION("0xC0", "[RET]") {
        SP_ = 0xC082;
        oldPC = PC_;
        memory_.write(0x12, PC_+1);
        memory_.write(0xC2, PC_+2);

        execute(0xCD);
        SECTION("shouldReturn") {
            FlagZ_.set(false);
            execute(0xC0);
            REQUIRE(PC_ == oldPC + unprefInstrArray_[0xCD].info_.getBytes());
            REQUIRE(SP_ == 0xC082);
            REQUIRE_FALSE(FlagZ_);
        }
        SECTION("shouldNotReturn") {
            FlagZ_.set(true);
            execute(0xC0);
            REQUIRE(PC_ == 0xC212 + unprefInstrArray_[0xC0].info_.getBytes());
        }
    }
    SECTION("0xC1, 0xD1, 0xE1", "[POP]") {
        std::vector<r16*> regPairs {&BC_, &DE_, &HL_};
        u8 k = 0xC1;
        for(u8 i = 0; i < regPairs.size(); i++, k+=16) {
            SP_ = 0xFFD0;
            memory.write(0xF0, SP_);
            memory.write(0x1F, SP_+1);
            execute(k);
            REQUIRE(SP_ == 0xFFD2);
            REQUIRE(*regPairs[i] == 0x1FF0);
        }
    }
    SECTION("0xC2", "[JP]") {
        memory_.write(0xF0, PC_+1);
        memory_.write(0xF1, PC_+2);
        SECTION("shouldJump") {
            execute(0xC2);
            REQUIRE(PC_ == 0xF1F0);
        }
        SECTION("shouldNotJump") {
            FlagZ_.set(true);
            execute(0xC2);
            REQUIRE_FALSE(PC_ == 0xF1F0);
        }
    }
    SECTION("0xC3", "[JP]") {
        memory_.write(0xF0, PC_+1);
        memory_.write(0xF1, PC_+2);
        execute(0xC3);
        REQUIRE(PC_ == 0xF1F0);
    }
    SECTION("0xC4", "[CALL]") {
            SP_ = 0xC082;
            oldPC = PC_ + 3; // PC += 3 is iniside call
            oldSP = SP_;
            memory_.write(0x12, PC_+1);
            memory_.write(0xC2, PC_+2);
        SECTION("shouldCall") {
            execute(0xC4);

            REQUIRE(SP_ == 0xC080);
            REQUIRE(PC_ == 0xC212);
            REQUIRE(fetch8(--oldSP) == oldPC.hi_);
            REQUIRE(fetch8(--oldSP) == oldPC.lo_);
        }
        SECTION("shouldNotCall") {
            FlagZ_.set(true);
            execute(0xC4);

            REQUIRE(SP_ == oldSP);
        }
    }
    SECTION("0xC5, 0xD5, 0xE5, 0xF5", "[PUSH]") {
        std::vector<r16*> regPairs {&BC_, &DE_, &HL_, &AF_};
        u8 k = 0xC5;
        for(u16 i = 0; i < regPairs.size(); i++, k+=16) {
            SP_ = 0xFFD3;
            *regPairs[i] = SP_+i;
            execute(k);
            REQUIRE(SP_ == 0xFFD1);
            REQUIRE(fetch8(SP_) == regPairs[i]->lo_);
            REQUIRE(fetch8(SP_+1) == regPairs[i]->hi_);
        }
    }
    SECTION("0xC6", "[ADD]") {
        A_ = 20;
        memory.write(20, PC_+1);
        execute(0xC6);
        REQUIRE(A_ == 40);

        A_ = 2;
        memory.write(0xFF, PC_+1);
        execute(0xC6);
        REQUIRE(A_ == 1);
    }
    SECTION("0xC7, 0xCF, 0xD7, 0xDF, 0xE7, 0xEF, 0xF7, 0xFF", "[RST]") {
        u8 op = 0xC7;
        u8 k = 0;

        for(u8 i = 0; i < 8; ++i, k+=8, op+=8) {
            PC_ = 0xC083;
            SP_ = 0xC082;
            oldPC = PC_ + 1; // PC_++ in RST
            oldSP = SP_;

            execute(op);
            REQUIRE(SP_ == 0xC080);
            REQUIRE(PC_.hi_ == 0x00);
            REQUIRE(PC_.lo_ == k);
            REQUIRE(fetch8(--oldSP) == oldPC.hi_);
            REQUIRE(fetch8(--oldSP) == oldPC.lo_);
        }
    }
    SECTION("0xC8", "[RET]") {
        SP_ = 0xC080;
        oldPC = PC_;
        oldSP = SP_;
        memory_.write(0x12, PC_+1);
        memory_.write(0xC2, PC_+2);

        execute(0xCD);
        SECTION("shouldReturn") {
            FlagZ_.set(true);
            u8 c = execute(0xC8);
            REQUIRE(PC_ == oldPC + unprefInstrArray_[0xCD].info_.getBytes());

            REQUIRE(c == 20);
        }
        SECTION("shouldNotReturn") {
            u8 c = execute(0xC8);
            REQUIRE(PC_ == 0xC212 + unprefInstrArray_[0xC8].info_.getBytes());
            REQUIRE(c == 8);
        }
    }
    SECTION("0xC9", "[RET]") {
        SP_ = 0xFFA0;
        oldPC = PC_;
        oldSP = SP_;
        memory_.write(0x12, PC_+1);
        memory_.write(0xC2, PC_+2);

        execute(0xCD);
        u8 c = execute(0xC9);

        REQUIRE(PC_ == oldPC + unprefInstrArray_[0xCD].info_.getBytes());
    }
    SECTION("0xCA", "[JP]") {
        memory_.write(0xF0, PC_+1);
        memory_.write(0xF1, PC_+2);
        SECTION("shouldJump") {
            FlagZ_.set(true);
            execute(0xCA);
            REQUIRE(PC_ == 0xF1F0);
        }
        SECTION("shouldNotJump") {
            execute(0xCA);
            REQUIRE_FALSE(PC_ == 0xF1F0);
        }
    }
    SECTION("0xCB") {
        REQUIRE_FALSE(isPrefixed_);
        execute(0xCB);
        REQUIRE(isPrefixed_);
        isPrefixed_ = false;
    }
    SECTION("0xCC", "[CALL]") {
        SP_ = 0xC082;
        oldPC = PC_ + 3; // PC += 3 is iniside call
        oldSP = SP_;
        memory_.write(0x12, PC_+1);
        memory_.write(0x02, PC_+2);
        SECTION("shouldCall") {
            FlagZ_.set(true);
            execute(0xCC);

            REQUIRE(SP_ == 0xC080);
            REQUIRE(PC_ == 0x0212);
            REQUIRE(fetch8(--oldSP) == oldPC.hi_);
            REQUIRE(fetch8(--oldSP) == oldPC.lo_);
        }
        SECTION("shouldNotCall") {
            execute(0xCC);

            REQUIRE(SP_ == oldSP);
        }
    }
    SECTION("0xCD", "[CALL]") {
        SP_ = 0xC082;
        oldPC = PC_ + 3; // PC += 3 is iniside call
        oldSP = SP_;
        memory_.write(0x12, PC_+1);
        memory_.write(0x02, PC_+2);

        execute(0xCD);

        REQUIRE(SP_ == 0xC080);
        REQUIRE(PC_ == 0x0212);
        REQUIRE(fetch8(--oldSP) == oldPC.hi_);
        REQUIRE(fetch8(--oldSP) == oldPC.lo_);
    }
    SECTION("0xCE", "[ADC]") {
        SECTION("FlagC_ = false") {
            FlagC_.set(false);
            A_ = 20;
            memory.write(20, PC_+1);
            execute(0xCE);
            REQUIRE(A_ == 40);

            FlagC_.set(false);
            A_ = 2;
            memory.write(0xFF, PC_+1);
            execute(0xCE);
            REQUIRE(FlagC_);
            REQUIRE(A_ == 1);
        }
        SECTION("FlagC_ = true") {
            FlagC_.set(true);
            A_ = 20;
            memory.write(20, PC_+1);
            execute(0xCE);
            REQUIRE(A_ == 41);

            FlagC_.set(true);
            A_ = 2;
            memory.write(0xFF, PC_+1);
            execute(0xCE);
            REQUIRE(A_ == 2);
        }
    }
    SECTION("0xD0", "[RET]") {
        SP_ = 0xC080;
        oldPC = PC_;
        oldSP = SP_;
        memory_.write(0x12, PC_+1);
        memory_.write(0xC2, PC_+2);

        execute(0xCD);
        SECTION("shouldReturn") {
            execute(0xD0);
            REQUIRE(PC_ == oldPC + unprefInstrArray_[0xCD].info_.getBytes());
        }
        SECTION("shouldNotReturn") {
            FlagC_.set(true);
            execute(0xD0);
            REQUIRE(PC_ == 0xC212 + unprefInstrArray_[0xD0].info_.getBytes());
        }
    }
    SECTION("0xD2", "[JP]") {
        memory_.write(0xF0, PC_+1);
        memory_.write(0xF1, PC_+2);
        SECTION("shouldJump") {
            execute(0xD2);
            REQUIRE(PC_ == 0xF1F0);
        }
        SECTION("shouldNotJump") {
            FlagC_.set(true);
            execute(0xD2);
            REQUIRE_FALSE(PC_ == 0xF1F0);
        }
    }
    SECTION("0xD4", "[JP]") {
        memory_.write(0xF0, PC_+1);
        memory_.write(0xF1, PC_+2);
        SECTION("shouldJump") {
            execute(0xD4);
            REQUIRE(PC_ == 0xF1F0);
        }
        SECTION("shouldNotJump") {
            FlagC_.set(true);
            execute(0xD4);
            REQUIRE_FALSE(PC_ == 0xF1F0);
        }
    }
    SECTION("0xD6", "[SUB]") {
        SECTION("shouldSubstractCorrectly") {
            A_ = 20;
            memory.write(20, PC_+1);
            execute(0xD6);
            REQUIRE(A_ == 0);

            A_ = 2;
            memory.write(0xFF, PC_+1);
            execute(0xD6);
            REQUIRE(A_ == 3);
        }
    }
    SECTION("0xD8", "[RET]") {
        SP_ = 0xC080;
        oldPC = PC_;
        oldSP = SP_;
        memory_.write(0x12, PC_+1);
        memory_.write(0xC2, PC_+2);

        execute(0xCD);
        SECTION("shouldReturn") {
            FlagC_.set(true);
            execute(0xD8);
            REQUIRE(PC_ == oldPC + unprefInstrArray_[0xCD].info_.getBytes());
        }
        SECTION("shouldNotReturn") {
            execute(0xD8);
            REQUIRE(PC_ == 0xC212 + unprefInstrArray_[0xD8].info_.getBytes());
        }
    }
    SECTION("0xD9", "[RETI]") {
        SP_ = 0xC080;
        oldPC = PC_;
        oldSP = SP_;
        memory_.write(0x12, PC_+1);
        memory_.write(0xC2, PC_+2);

        execute(0xCD);
        execute(0xD9);
        REQUIRE(PC_ == oldPC + unprefInstrArray_[0xCD].info_.getBytes());
        REQUIRE(IME_);
    }
    SECTION("0xDA", "[JP]") {
        memory_.write(0xF0, PC_+1);
        memory_.write(0xF1, PC_+2);
        SECTION("shouldJump") {
            FlagC_.set(true);
            execute(0xDA);
            REQUIRE(PC_ == 0xF1F0);
        }
        SECTION("shouldNotJump") {
            execute(0xDA);
            REQUIRE_FALSE(PC_ == 0xF1F0);
        }
    }
    SECTION("0xDC", "[CALL]") {
        SP_ = 0xC082;
        oldPC = PC_ + 3; // PC += 3 is iniside call
        oldSP = SP_;
        memory_.write(0x12, PC_+1);
        memory_.write(0x02, PC_+2);
        SECTION("shouldCall") {
            FlagC_.set(true);

            execute(0xDC);

            REQUIRE(SP_ == 0xC080);
            REQUIRE(PC_ == 0x0212);
            REQUIRE(fetch8(--oldSP) == oldPC.hi_);
            REQUIRE(fetch8(--oldSP) == oldPC.lo_);
        }
        SECTION("shouldNotCall") {
            execute(0xDC);

            REQUIRE(SP_ == oldSP);
        }
    }
    SECTION("0xDE", "[SBC]") {
        A_ = 20;
        memory.write(20, PC_+1);
        execute(0xDE);
        REQUIRE(A_ == 0);

        FlagC_.set(true);
        A_ = 2;
        memory.write(0xFF, PC_+1);
        execute(0xDE);
        REQUIRE(A_ == 2);
    }
    SECTION("0xDF", "[LDD]") {
        A_ = 0x90;
        memory_.write(A_, PC_+1);
        execute(0xE0);
        REQUIRE(fetch8(0xFF00 + A_) == A_);
    }
    SECTION("0xE2", "[LDD]") {
        A_ = 20;
        C_ = 0x90;
        execute(0xE2);
        REQUIRE(fetch8(0xFF00 + C_) == A_);
    }
    SECTION("0xE6", "[AND]") {
        A_ =  0b11100101;
        memory.write(0b11000111, PC_+1);
        execute(0xE6);
        REQUIRE(A_ == 0b11000101);
    }
    SECTION("0xE8", "[ADDSP]") {
        SECTION("shouldAddPositiveNumber") {
            SP_ = 30000;
            memory.write(40, PC_+1);
            execute(0xE8);
            REQUIRE(SP_ == 30040);
        }
        SECTION("shouldAddNegativeNumber") {
            SP_ = 30000;
            memory.write(-40, PC_+1);
            execute(0xE8);
            REQUIRE(SP_ == 29960);
        }
        SECTION("shouldSetHalfCarryFlag") {
            SP_ = 0x0008;
            memory.write(0x08, PC_+1);
            execute(0xE8);
            REQUIRE(FlagH_);
        }
        SECTION("shouldSetHalfCarryFlag") {
            SP_ = 0x00FF;
            memory.write(1, PC_+1);
            execute(0xE8);
            REQUIRE(FlagC_);
        }
    }
    SECTION("0xE9", "[JP]") {
        HL_ = 0xC1F0;
        execute(0xE9);
        REQUIRE(PC_ == 0xC1F0);
    }
    SECTION("0xEA", "[LDD]") {
        A_ = 20;
        memory.write(0x80, PC_+1);
        memory.write(0xC0, PC_+2);
        execute(0xEA);
        REQUIRE(fetch8(0xC080) == A_);
    }
    SECTION("0xEE") {
        A_ =  0b11100101;
        memory.write(0b11000111, PC_+1);
        execute(0xEE);
        REQUIRE(A_ == 0b00100010);
    }
    SECTION("0xF0", "[LD]") {
        u8 val = 0x90;
        memory_.write(val, PC_+1);
        memory_.write(val, 0xFF00 + fetch8(PC_+1));
        execute(0xF0);
        REQUIRE(A_ == val);
    }
    SECTION("0xF1", "[popAF]") {
        SP_ = 0xFFF0;
        memory_.write(0xF0, SP_);
        memory_.write(0xF1, SP_+1);
        execute(0xF1);
        REQUIRE(AF_ == 0xF1F0);
        REQUIRE(FlagZ_);
        REQUIRE(FlagN_);
        REQUIRE(FlagH_);
        REQUIRE(FlagC_);
    }
    SECTION("0xF2", "[LD]") {
        u8 val = 40;
        C_ = 0x90;
        memory_.write(val, 0xFF00 + C_);

        execute(0xF2);
        REQUIRE(A_ == val);
    }
    SECTION("0xF3", "[DI]") {
        IME_ = true;
        ic_.setIE(0);
        REQUIRE_FALSE(isDISet_);

        execute(0xF3);

        REQUIRE(IME_);
        REQUIRE(isDISet_);

        execute(0x00);

        REQUIRE_FALSE(IME_);
    }
    SECTION("0xF6", "[OR]") {
        A_ =  0b11100101;
        memory.write(0b11000111, PC_+1);
        execute(0xF6);
        REQUIRE(A_ == 0b11100111);
    }
    SECTION("0xF8") {
        int8_t val;
        SECTION("positive") {
            SECTION("shouldSetHalfCarryFlag") {
                val = 8;
                SP_ = 8;
                memory_.write(val, PC_+1);
                execute(0xF8);
                REQUIRE(HL_ == SP_ + val);
                REQUIRE(FlagH_);
            }
            SECTION("shouldNotSetHalfCarryFlag") {
                val = 64;
                SP_ = 32;
                memory_.write(val, PC_+1);
                execute(0xF8);
                REQUIRE(HL_ == SP_ + val);
                REQUIRE_FALSE(FlagH_);
            }
            SECTION("shouldSetCarryFlag") {
                val = 0b11111111;
                SP_ = 1;
                memory_.write(val, PC_+1);
                execute(0xF8);
                REQUIRE(HL_ == SP_ + val);
                REQUIRE(FlagC_);
            }
            SECTION("shouldNotSetCarryFlag") {
                val = 0b01111111;
                SP_ = 1;
                memory_.write(val, PC_+1);
                execute(0xF8);
                REQUIRE(HL_ == SP_ + val);
                REQUIRE_FALSE(FlagC_);
            }
        }
        SECTION("negative") {
            SECTION("shouldSetHalfCarryFlag") {
                val = 0b10001000;
                SP_ = 0b00001000;
                memory_.write(val, PC_+1);
                execute(0xF8);
                REQUIRE(HL_ == SP_ + val);
                REQUIRE(FlagH_);
            }
            SECTION("shouldNotSetHalfCarryFlag") {
                val = 64;
                SP_ = 32;
                memory_.write(val, PC_+1);
                execute(0xF8);
                REQUIRE(HL_ == SP_ + val);
                REQUIRE_FALSE(FlagH_);
            }
            SECTION("shouldSetCarryFlag") {
                val = -3;
                SP_ = 0xFFFF;
                memory_.write(val, PC_+1);
                execute(0xF8);
                REQUIRE(HL_ == SP_ + val);
                REQUIRE(FlagC_);
            }
            SECTION("shouldNotSetCarryFlag") {
                val = 0;
                SP_ = -1;
                memory_.write(val, PC_+1);
                execute(0xF8);
                REQUIRE(HL_ == SP_ + val);
                REQUIRE_FALSE(FlagC_);
            }
            val = -1;
            SP_ = 2;
            memory_.write(val, PC_+1);
            execute(0xF8);
            REQUIRE(HL_ == 1);

            val = -3;
            SP_ = 2;
            memory_.write(val, PC_+1);
            execute(0xF8);
            REQUIRE(HL_ == 0xFFFF);
        }
    }
    SECTION("0xF9", "[LD16]") {
        HL_ = 0b1101110111001101;
        execute(0xF9);
        REQUIRE(SP_ == HL_);
    }
    SECTION("0xFA", "[LD]") {
        u8 val = 20;
        memory_.write(0xFA, PC_+1);
        memory_.write(0xC1, PC_+2);
        memory_.write(val, 0xC1FA);

        execute(0xFA);
        REQUIRE(A_ == val);
    }
    SECTION("0xFB", "[EI]") {
        IME_ = false;
        isEISet_ = false;

        execute(0xFB);

        REQUIRE_FALSE(IME_);
        REQUIRE(isEISet_);

        execute(0x00);

        REQUIRE(IME_);
    }
    SECTION("0xFE") {
        A_ =  21;
        memory.write(23, PC_+1);
        execute(0xFE);
        REQUIRE(A_ == 21);
    }
}
