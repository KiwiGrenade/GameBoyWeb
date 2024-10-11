#include "CPU.hpp"

#include <catch2/catch_test_macros.hpp>
#include <vector>

struct ProceduresUnprefixedTests : CPU {
    ProceduresUnprefixedTests() : CPU(memory) {
        /*for(size_t i = 0; i < memory.size_; i++) {*/
        /*    memory.write(u8(i), i);*/
        /*}*/
        F_ = 0;
    };
    Memory memory;

    void execute(u8 op) {
        memory_.write(op, PC_);
        step();
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
        BC_ = 11;
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
        std::vector<r8*> reg{&B_, &C_, &D_, &E_, &H_, &L_, &A_};
        u8 k = 0x04;
        for(u16 i = 0; i < reg.size(); i++, k+=8) {
            if(i == 6)
                continue;
            *reg[i] = i;
            execute(k);
            REQUIRE(*reg[i] - 1 == i);
        }
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
        
        std::vector<r8*> reg{&B_, &C_, &D_, &E_, &H_, &L_, &A_};
        u8 k = 0x05;
        for(u16 i = 0; i < reg.size(); i++, k+=8) {
            if(i == 6)
                continue;
            *reg[i] = i;
            execute(k);
            REQUIRE(u8(*reg[i] + 1) == i);
        }
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
        std::vector<u8*> reg {&B_, &C_, &D_, &E_, &H_, &L_, &A_};
        u8 k = 0x06;
        for(u8 i = 0; i < 8; i++, k +=8) {
            if(i == 6)
                continue;
            u8 op = k;
            u8* to = i == 7 ? reg[i-1] : reg[i];
            u16 addrU8 = PC_+1;
            memory_.write(op, addrU8);
            execute(op);
            REQUIRE(fetch8(addrU8) == *to);
        }
    }
    SECTION("0x07", "[RLCA]") {
        A_ = 0b10000001;
        FlagC_.set(false);

        execute(0x07);
        REQUIRE(FlagC_);
        REQUIRE(00000011);
    }
    SECTION("0x08", "[LDD16]") {
        SP_ = 28;
        memory_.write(0x0010, PC_+1);
        memory_.write(0x0000, PC_+2);
        execute(0x08);
        REQUIRE(fetch8(0x00000010) == SP_);
    }
    /*SECTION("0x09") {*/
    /*    step();*/
    /*    REQUIRE(true);*/
    /*}*/
    SECTION("0x0A", "[LD]") {
        BC_ = 13;
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
    /*SECTION("0x0C") {*/
    /*    step();*/
    /*    REQUIRE(true);*/
    /*}*/
    /*SECTION("0x0D") {*/
    /*    step();*/
    /*    REQUIRE(true);*/
    /*}*/
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
    /*SECTION("0x11") {*/
    /*    step();*/
    /*    REQUIRE(true);*/
    /*}*/
    SECTION("0x12", "[LDD]") {
        DE_ = 15;
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
            REQUIRE(PC_ == oldPC+2+20);
        }
        SECTION("negative") {
            memory_.write(int8_t(-20), PC_+1);
            execute(0x18);
            REQUIRE(PC_ == oldPC.getVal()+2-20);
        }
    }
    /*SECTION("0x19") {*/
    /*    step();*/
    /*    REQUIRE(true);*/
    /*}*/
    SECTION("0x1A", "[LD]") {
        DE_ = 17;
        u8 val = 18;
        memory_.write(val, DE_);
        execute(0x1A);
        REQUIRE(A_ == val);
    }
    /*SECTION("0x1C") {*/
    /*    step();*/
    /*    REQUIRE(true);*/
    /*}*/
    /*SECTION("0x1D") {*/
    /*    step();*/
    /*    REQUIRE(true);*/
    /*}*/
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
            REQUIRE(PC_ == oldPC+2+val);
        }
        SECTION("shouldNotJump") {
            FlagZ_.set(true);
            memory_.write(int8_t(val), PC_+1);
            execute(op);
            REQUIRE(PC_ == oldPC+2);
        }
    }
    /*SECTION("0x21") {*/
    /*    step();*/
    /*    REQUIRE(true);*/
    /*}*/
    SECTION("0x22", "[LDD]") {
        HL_ = 17;
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
            REQUIRE(PC_ == oldPC+2+val);
        }
        SECTION("shouldNotJump") {
            memory_.write(int8_t(val), PC_+1);
            execute(op);
            REQUIRE(PC_ == oldPC+2);
        }
    }
    /*SECTION("0x29") {*/
    /*    step();*/
    /*    REQUIRE(true);*/
    /*}*/
    SECTION("0x2A", "[LD]") {
        HL_ = 19;
        u16 oldHL = HL_;
        u8 val = 20;
        memory.write(val, HL_);
        execute(0x2A);
        REQUIRE(oldHL + 1 == HL_);
        REQUIRE(A_ == val);
    }
    /*SECTION("0x2C") {*/
    /*    step();*/
    /*    REQUIRE(true);*/
    /*}*/
    /*SECTION("0x2D") {*/
    /*    step();*/
    /*    REQUIRE(true);*/
    /*}*/
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
            REQUIRE(PC_ == oldPC+2+val);
        }
        SECTION("shouldNotJump") {
            FlagC_.set(true);
            memory_.write(int8_t(val), PC_+1);
            execute(op);
            REQUIRE(PC_ == oldPC+2);
        }
    }
    /*SECTION("0x31") {*/
    /*    step();*/
    /*    REQUIRE(true);*/
    /*}*/
    SECTION("0x32", "[LDD]") {
        HL_ = 21;
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
        HL_ = 10;
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
            REQUIRE(PC_ == oldPC+2+val);
        }
        SECTION("shouldNotJump") {
            memory_.write(int8_t(val), PC_+1);
            execute(op);
            REQUIRE(PC_ == oldPC+2);
        }
    }
    /*SECTION("0x39") {*/
    /*    step();*/
    /*    REQUIRE(true);*/
    /*}*/
    SECTION("0x3A", "[LD]") {
        HL_ = 23;
        u16 oldHL = HL_;
        u8 val = 24;
        memory.write(val, HL_);
        execute(0x3A);
        REQUIRE(oldHL - 1 == HL_);
        REQUIRE(A_ == val);
    }
    /*SECTION("0x3C") {*/
    /*    step();*/
    /*    REQUIRE(true);*/
    /*}*/
    /*SECTION("0x3D") {*/
    /*    step();*/
    /*    REQUIRE(true);*/
    /*}*/
    SECTION("0x3F", "[CCF]") {
        execute(0x3f);
        REQUIRE(oldFlagC != FlagC_);
        execute(0x3f);
        REQUIRE(oldFlagC == FlagC_);
    }
    SECTION("0x40-0x7F", "[LD], [LDD], [HALT]") {
        SECTION("0x76", "[HALT]") {
            isHalted_ = false;
            IME_ = true;
            execute(0x76);
            REQUIRE(isHalted_);

            isHalted_ = false;
            IME_ = false;
            IE_ = 0;
            IF_ = 1;
            execute(0x76);
            REQUIRE(isHalted_);

            isHalted_ = false;
            IME_ = false;
            IE_ = 1;
            IF_ = 1;
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
            k = 0x70;
            for(u8 i = 0; i < 8; i++) {
                if(i == 6)
                    continue;
                op = k + i;
                u8* from = i == 7 ? reg[i-1] : reg[i];
                *from = op;
                HL_.setVal(op);
                execute(op);
                REQUIRE(fetch8(HL_) == *from);
            }
            REQUIRE(fetch8(0x77) == 0x77);
        }
        SECTION("0x46, 0x4E, 0x56, 0x5E, 0x66, 0x6E, 0x77, 0x7E") {

            k = 0x46;
            for(u8 i = 0; i < 8; i++, k +=8) {
                if(i == 6)
                    continue;
                op = k;
                u8* to = i == 7 ? reg[i-1] : reg[i];
                HL_.setVal(op);
                memory_.write(op, HL_);
                u16 prevHL_ = HL_; // execute changes H_ and L_ registers
                execute(op);
                REQUIRE(fetch8(prevHL_) == *to);
            }
        }
    }
    SECTION("0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x87", "[ADD]") {
        std::vector<u8*> reg {&B_, &C_, &D_, &E_, &H_, &L_, &A_};
        u8 k = 0x80;
        for(u8 i = 0; i < reg.size(); ++i) {
            if(i == 6)
                continue;
            u8* to = i == 7 ? reg[i-1] : reg[i];

            A_ = k;
            *to = i;
            execute(k+i);
            REQUIRE(A_ == k+i);

            A_ = 2;
            *to = 0xFF;
            execute(k+i);
            REQUIRE(A_ == 1);
        }
    }
    SECTION("0x86", "[ADD]") {
        A_ = 20;
        memory.write(20, HL_);
        execute(0x86);
        REQUIRE(A_ == 40);

        A_ = 2;
        memory.write(0xFF, HL_);
        execute(0x86);
        REQUIRE(A_ == 1);
    }
    SECTION("0x88, 0x89, 0x8A, 0x8B, 0x8C, 0x8D, 0x8F", "[ADD]") {
        std::vector<u8*> reg {&B_, &C_, &D_, &E_, &H_, &L_, &A_};
        u8 k = 0x88;
        for(u8 i = 0; i < reg.size(); ++i) {
            FlagC_.set(true);
            if(i == 6)
                continue;
            u8* to = i == 7 ? reg[i-1] : reg[i];

            A_ = k;
            *to = i;
            execute(k+i);
            //FIXME: fix this
            REQUIRE(A_ == k+i+FlagC_);

            A_ = 2;
            *to = 0xFF;
            execute(k+i);
            REQUIRE(A_ == 1 + FlagC_);
        }
        for(u8 i = 0; i < reg.size(); ++i) {
            FlagC_.set(false);
            if(i == 6)
                continue;
            u8* to = i == 7 ? reg[i-1] : reg[i];

            A_ = k;
            *to = i;
            execute(k+i);
            REQUIRE(A_ == k+i+FlagC_);

            A_ = 2;
            *to = 0xFF;
            execute(k+i);
            REQUIRE(A_ == 1 + FlagC_);
        }
    }
    SECTION("0x8E", "[ADD]") {
        FlagC_.set(true);
        A_ = 20;
        memory.write(20, HL_);
        execute(0x8E);
        REQUIRE(A_ == 41);

        FlagC_.set(true);
        A_ = 2;
        memory.write(0xFF, HL_);
        execute(0x8E);
        REQUIRE(A_ == 1 + FlagC_);

        FlagC_.set(false);
        A_ = 20;
        memory.write(20, HL_);
        execute(0x8E);
        REQUIRE(A_ == 40);

        FlagC_.set(false);
        A_ = 2;
        memory.write(0xFF, HL_);
        execute(0x8E);
        //FIXME: Fix this
        REQUIRE(A_ == 1 + FlagC_);
    }
    /*SECTION("0x88") {*/
    /*    step();*/
    /*    REQUIRE(true);*/
    /*}*/
    /*SECTION("0x89") {*/
    /*    step();*/
    /*    REQUIRE(true);*/
    /*}*/
    /*SECTION("0x8A") {*/
    /*    step();*/
    /*    REQUIRE(true);*/
    /*}*/
    /*SECTION("0x8B") {*/
    /*    step();*/
    /*    REQUIRE(true);*/
    /*}*/
    /*SECTION("0x8C") {*/
    /*    step();*/
    /*    REQUIRE(true);*/
    /*}*/
    /*SECTION("0x8D") {*/
    /*    step();*/
    /*    REQUIRE(true);*/
    /*}*/
    /*SECTION("0x8E") {*/
    /*    step();*/
    /*    REQUIRE(true);*/
    /*}*/
    /*SECTION("0x8F") {*/
    /*    step();*/
    /*    REQUIRE(true);*/
    /*}*/
    /*SECTION("0x90") {*/
    /*    step();*/
    /*    REQUIRE(true);*/
    /*}*/
    /*SECTION("0x91") {*/
    /*    step();*/
    /*    REQUIRE(true);*/
    /*}*/
    /*SECTION("0x92") {*/
    /*    step();*/
    /*    REQUIRE(true);*/
    /*}*/
    /*SECTION("0x93") {*/
    /*    step();*/
    /*    REQUIRE(true);*/
    /*}*/
    /*SECTION("0x94") {*/
    /*    step();*/
    /*    REQUIRE(true);*/
    /*}*/
    /*SECTION("0x95") {*/
    /*    step();*/
    /*    REQUIRE(true);*/
    /*}*/
    /*SECTION("0x96") {*/
    /*    step();*/
    /*    REQUIRE(true);*/
    /*}*/
    /*SECTION("0x97") {*/
    /*    step();*/
    /*    REQUIRE(true);*/
    /*}*/
    /*SECTION("0x98") {*/
    /*    step();*/
    /*    REQUIRE(true);*/
    /*}*/
    /*SECTION("0x99") {*/
    /*    step();*/
    /*    REQUIRE(true);*/
    /*}*/
    /*SECTION("0x9A") {*/
    /*    step();*/
    /*    REQUIRE(true);*/
    /*}*/
    /*SECTION("0x9B") {*/
    /*    step();*/
    /*    REQUIRE(true);*/
    /*}*/
    /*SECTION("0x9C") {*/
    /*    step();*/
    /*    REQUIRE(true);*/
    /*}*/
    /*SECTION("0x9D") {*/
    /*    step();*/
    /*    REQUIRE(true);*/
    /*}*/
    /*SECTION("0x9E") {*/
    /*    step();*/
    /*    REQUIRE(true);*/
    /*}*/
    /*SECTION("0x9F") {*/
    /*    step();*/
    /*    REQUIRE(true);*/
    /*}*/
    /*SECTION("0xA0") {*/
    /*    step();*/
    /*    REQUIRE(true);*/
    /*}*/
    /*SECTION("0xA1") {*/
    /*    step();*/
    /*    REQUIRE(true);*/
    /*}*/
    /*SECTION("0xA2") {*/
    /*    step();*/
    /*    REQUIRE(true);*/
    /*}*/
    /*SECTION("0xA3") {*/
    /*    step();*/
    /*    REQUIRE(true);*/
    /*}*/
    /*SECTION("0xA4") {*/
    /*    step();*/
    /*    REQUIRE(true);*/
    /*}*/
    /*SECTION("0xA5") {*/
    /*    step();*/
    /*    REQUIRE(true);*/
    /*}*/
    /*SECTION("0xA6") {*/
    /*    step();*/
    /*    REQUIRE(true);*/
    /*}*/
    /*SECTION("0xA7") {*/
    /*    step();*/
    /*    REQUIRE(true);*/
    /*}*/
    /*SECTION("0xA8") {*/
    /*    step();*/
    /*    REQUIRE(true);*/
    /*}*/
    /*SECTION("0xA9") {*/
    /*    step();*/
    /*    REQUIRE(true);*/
    /*}*/
    /*SECTION("0xAA") {*/
    /*    step();*/
    /*    REQUIRE(true);*/
    /*}*/
    /*SECTION("0xAB") {*/
    /*    step();*/
    /*    REQUIRE(true);*/
    /*}*/
    /*SECTION("0xAC") {*/
    /*    step();*/
    /*    REQUIRE(true);*/
    /*}*/
    /*SECTION("0xAD") {*/
    /*    step();*/
    /*    REQUIRE(true);*/
    /*}*/
    /*SECTION("0xAE") {*/
    /*    step();*/
    /*    REQUIRE(true);*/
    /*}*/
    /*SECTION("0xAF") {*/
    /*    step();*/
    /*    REQUIRE(true);*/
    /*}*/
    /*SECTION("0xB0") {*/
    /*    step();*/
    /*    REQUIRE(true);*/
    /*}*/
    /*SECTION("0xB1") {*/
    /*    step();*/
    /*    REQUIRE(true);*/
    /*}*/
    /*SECTION("0xB2") {*/
    /*    step();*/
    /*    REQUIRE(true);*/
    /*}*/
    /*SECTION("0xB3") {*/
    /*    step();*/
    /*    REQUIRE(true);*/
    /*}*/
    /*SECTION("0xB4") {*/
    /*    step();*/
    /*    REQUIRE(true);*/
    /*}*/
    /*SECTION("0xB5") {*/
    /*    step();*/
    /*    REQUIRE(true);*/
    /*}*/
    /*SECTION("0xB6") {*/
    /*    step();*/
    /*    REQUIRE(true);*/
    /*}*/
    /*SECTION("0xB7") {*/
    /*    step();*/
    /*    REQUIRE(true);*/
    /*}*/
    /*SECTION("0xB8") {*/
    /*    step();*/
    /*    REQUIRE(true);*/
    /*}*/
    /*SECTION("0xB9") {*/
    /*    step();*/
    /*    REQUIRE(true);*/
    /*}*/
    /*SECTION("0xBA") {*/
    /*    step();*/
    /*    REQUIRE(true);*/
    /*}*/
    /*SECTION("0xBB") {*/
    /*    step();*/
    /*    REQUIRE(true);*/
    /*}*/
    /*SECTION("0xBC") {*/
    /*    step();*/
    /*    REQUIRE(true);*/
    /*}*/
    /*SECTION("0xBD") {*/
    /*    step();*/
    /*    REQUIRE(true);*/
    /*}*/
    /*SECTION("0xBE") {*/
    /*    step();*/
    /*    REQUIRE(true);*/
    /*}*/
    /*SECTION("0xBF") {*/
    /*    step();*/
    /*    REQUIRE(true);*/
    /*}*/
    SECTION("0xC0, 0xC4, 0xC8, 0xC9, 0xCC, 0xCD, 0xD0, 0xD8, 0xD9, 0xDA, 0xDC", "[RET]") {
        SP_ = 0xFF82;
        PC_ = 0x0130;
        oldPC = PC_;
        oldSP = SP_;
        memory_.write(0x12, PC_+1);
        memory_.write(0x02, PC_+2);

        execute(0xCD);
        SECTION("shouldReturn") {
            execute(0xC0);
            REQUIRE(PC_ == oldPC);
        }
        SECTION("shouldNotReturn") {
            FlagZ_.set(true);
            execute(0xC0);
            REQUIRE_FALSE(PC_ == oldPC);
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
            SP_ = 0xFF82;
            PC_ = 0x0130;
            oldPC = PC_;
            oldSP = SP_;
            memory_.write(0x12, PC_+1);
            memory_.write(0x02, PC_+2);
        SECTION("shouldCall") {
            execute(0xC4);

            REQUIRE(SP_ == 0xFF80);
            REQUIRE(PC_ == 0x0212);
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
    /*SECTION("0xC6") {*/
    /*    step();*/
    /*    REQUIRE(true);*/
    /*}*/
    SECTION("0xC7", "[RST]") {
        u8 op = 0xC7;
        u8 k = 0;

        for(u8 i = 0; i < 8; ++i, k+=8, op+=8) {
            SP_ = 0xFF82;
            PC_ = 0x0130;
            oldPC = PC_;
            oldSP = SP_;

            execute(op);
            REQUIRE(SP_ == 0xFF80);
            REQUIRE(PC_.hi_ == 0x00);
            REQUIRE(PC_.lo_ == k);
            REQUIRE(fetch8(--oldSP) == oldPC.hi_);
            REQUIRE(fetch8(--oldSP) == oldPC.lo_);
        }
    }
    SECTION("0xC8", "[RET]") {
        SP_ = 0xFF82;
        PC_ = 0x0130;
        oldPC = PC_;
        oldSP = SP_;
        memory_.write(0x12, PC_+1);
        memory_.write(0x02, PC_+2);

        execute(0xCD);
        SECTION("shouldReturn") {
            FlagZ_.set(true);
            execute(0xC8);
            REQUIRE(PC_ == oldPC);
        }
        SECTION("shouldNotReturn") {
            execute(0xC8);
            REQUIRE_FALSE(PC_ == oldPC);
        }
    }
    SECTION("0xC9", "[RET]") {
        SP_ = 0xFF82;
        PC_ = 0x0130;
        oldPC = PC_;
        oldSP = SP_;
        memory_.write(0x12, PC_+1);
        memory_.write(0x02, PC_+2);

        execute(0xCD);
        execute(0xC9);

        REQUIRE(PC_ == oldPC);
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
    /*SECTION("0xCB") {*/
    /*    step();*/
    /*    REQUIRE(true);*/
    /*}*/
    SECTION("0xCC", "[CALL]") {
            SP_ = 0xFF82;
            PC_ = 0x0130;
            oldPC = PC_;
            oldSP = SP_;
            memory_.write(0x12, PC_+1);
            memory_.write(0x02, PC_+2);
        SECTION("shouldCall") {
            FlagZ_.set(true);
            execute(0xCC);

            REQUIRE(SP_ == 0xFF80);
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
        SP_ = 0xFF82;
        PC_ = 0x0130;
        oldPC = PC_;
        oldSP = SP_;
        memory_.write(0x12, PC_+1);
        memory_.write(0x02, PC_+2);

        execute(0xCD);

        REQUIRE(SP_ == 0xFF80);
        REQUIRE(PC_ == 0x0212);
        REQUIRE(fetch8(--oldSP) == oldPC.hi_);
        REQUIRE(fetch8(--oldSP) == oldPC.lo_);
    }
    /*SECTION("0xCE") {*/
    /*    step();*/
    /*    REQUIRE(true);*/
    /*}*/
    /*SECTION("0xCF") {*/
    /*    step();*/
    /*    REQUIRE(true);*/
    /*}*/
    SECTION("0xD0", "[RET]") {
        SP_ = 0xFF82;
        PC_ = 0x0130;
        oldPC = PC_;
        oldSP = SP_;
        memory_.write(0x12, PC_+1);
        memory_.write(0x02, PC_+2);

        execute(0xCD);
        SECTION("shouldReturn") {
            execute(0xD0);
            REQUIRE(PC_ == oldPC);
        }
        SECTION("shouldNotReturn") {
            FlagC_.set(true);
            execute(0xD0);
            REQUIRE_FALSE(PC_ == oldPC);
        }
    }
    /*SECTION("0xD2") {*/
    /*    step();*/
    /*    REQUIRE(true);*/
    /*}*/
    /*SECTION("0xD3") {*/
    /*    step();*/
    /*    REQUIRE(true);*/
    /*}*/
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
    /*SECTION("0xD6") {*/
    /*    step();*/
    /*    REQUIRE(true);*/
    /*}*/
    /*SECTION("0xD7") {*/
    /*    step();*/
    /*    REQUIRE(true);*/
    /*}*/
    SECTION("0xD8", "[RET]") {
        SP_ = 0xFF82;
        PC_ = 0x0130;
        oldPC = PC_;
        oldSP = SP_;
        memory_.write(0x12, PC_+1);
        memory_.write(0x02, PC_+2);

        execute(0xCD);
        SECTION("shouldReturn") {
            FlagC_.set(true);
            execute(0xD8);
            REQUIRE(PC_ == oldPC);
        }
        SECTION("shouldNotReturn") {
            execute(0xD8);
            REQUIRE_FALSE(PC_ == oldPC);
        }
    }
    SECTION("0xD9", "[RETI]") {
        SP_ = 0xFF82;
        PC_ = 0x0130;
        oldPC = PC_;
        oldSP = SP_;
        memory_.write(0x12, PC_+1);
        memory_.write(0x02, PC_+2);

        execute(0xCD);
        execute(0xD9);
        REQUIRE(PC_ == oldPC);
        REQUIRE(IME_);
    }
    SECTION("0xDA", "[CALL]") {
        SP_ = 0xFF82;
        PC_ = 0x0130;
        oldPC = PC_;
        oldSP = SP_;
        memory_.write(0x12, PC_+1);
        memory_.write(0x02, PC_+2);
        SECTION("shouldCall") {
            execute(0xDA);

            REQUIRE(SP_ == 0xFF80);
            REQUIRE(PC_ == 0x0212);
            REQUIRE(fetch8(--oldSP) == oldPC.hi_);
            REQUIRE(fetch8(--oldSP) == oldPC.lo_);
        }
        SECTION("shouldNotCall") {
            FlagC_.set(true);
            execute(0xDA);

            REQUIRE(SP_ == oldSP);
        }
    }
    /*SECTION("0xDB") {*/
    /*    step();*/
    /*    REQUIRE(true);*/
    /*}*/
    SECTION("0xDC", "[CALL]") {
        SP_ = 0xFF82;
        PC_ = 0x0130;
        oldPC = PC_;
        oldSP = SP_;
        memory_.write(0x12, PC_+1);
        memory_.write(0x02, PC_+2);
        SECTION("shouldCall") {
            FlagC_.set(true);

            execute(0xDC);

            REQUIRE(SP_ == 0xFF80);
            REQUIRE(PC_ == 0x0212);
            REQUIRE(fetch8(--oldSP) == oldPC.hi_);
            REQUIRE(fetch8(--oldSP) == oldPC.lo_);
        }
        SECTION("shouldNotCall") {
            execute(0xDC);

            REQUIRE(SP_ == oldSP);
        }
    }
    /*SECTION("0xDD") {*/
    /*    step();*/
    /*    REQUIRE(true);*/
    /*}*/
    /*SECTION("0xDE") {*/
    /*    step();*/
    /*    REQUIRE(true);*/
    /*}*/
    /*SECTION("0xDF") {*/
    /*    step();*/
    /*    REQUIRE(true);*/
    /*}*/
    SECTION("0xDF", "[LDD]") {
        A_ = 20;
        memory_.write(A_, PC_+1);
        execute(0xE0);
        REQUIRE(fetch8(0xFF00 + A_) == A_);
    }
    SECTION("0xE2", "[LDD]") {
        A_ = 20;
        C_ = 3;
        execute(0xE2);
        REQUIRE(fetch8(0xFF00 + C_) == A_);
    }
    /*SECTION("0xE3") {*/
    /*    step();*/
    /*    REQUIRE(true);*/
    /*}*/
    /*SECTION("0xE4") {*/
    /*    step();*/
    /*    REQUIRE(true);*/
    /*}*/
    /*SECTION("0xE6") {*/
    /*    step();*/
    /*    REQUIRE(true);*/
    /*}*/
    /*SECTION("0xE7") {*/
    /*    step();*/
    /*    REQUIRE(true);*/
    /*}*/
    /*SECTION("0xE8") {*/
    /*    step();*/
    /*    REQUIRE(true);*/
    /*}*/
    SECTION("0xE9", "[JP]") {
        HL_ = 0xF1F0;
        execute(0xE9);
        REQUIRE(PC_ == 0xF1F0);
    }
    SECTION("0xEA", "[LDD]") {
        A_ = 20;
        execute(0xEA);
        REQUIRE(fetch8(fetch16(oldPC+1)) == A_);
    }
    /*SECTION("0xEB") {*/
    /*    step();*/
    /*    REQUIRE(true);*/
    /*}*/
    /*SECTION("0xEC") {*/
    /*    step();*/
    /*    REQUIRE(true);*/
    /*}*/
    /*SECTION("0xED") {*/
    /*    step();*/
    /*    REQUIRE(true);*/
    /*}*/
    /*SECTION("0xEE") {*/
    /*    step();*/
    /*    REQUIRE(true);*/
    /*}*/
    /*SECTION("0xEF") {*/
    /*    step();*/
    /*    REQUIRE(true);*/
    /*}*/
    SECTION("0xF0", "[LD]") {
        u8 val = 20;
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
        C_ = 4;
        memory_.write(val, 0xFF00 + C_);

        execute(0xF2);
        REQUIRE(A_ == val);
    }
    SECTION("0xF3", "[DI]") {
        IME_ = true;
        REQUIRE_FALSE(isDISet_);

        execute(0xF3);

        REQUIRE(IME_);
        REQUIRE(isDISet_);

        execute(0x00);

        REQUIRE_FALSE(IME_);
    }
    /*SECTION("0xF4") {*/
    /*    step();*/
    /*    REQUIRE(true);*/
    /*}*/
    /*SECTION("0xF6") {*/
    /*    step();*/
    /*    REQUIRE(true);*/
    /*}*/
    /*SECTION("0xF7") {*/
    /*    step();*/
    /*    REQUIRE(true);*/
    /*}*/
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
        memory_.write(0x01, PC_+2);
        memory_.write(val, 0x01FA);

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
    /*SECTION("0xFC") {*/
    /*    step();*/
    /*    REQUIRE(true);*/
    /*}*/
    /*SECTION("0xFD") {*/
    /*    step();*/
    /*    REQUIRE(true);*/
    /*}*/
    /*SECTION("0xFE") {*/
    /*    step();*/
    /*    REQUIRE(true);*/
    /*}*/
    /*SECTION("0xFF") {*/
    /*    step();*/
    /*    REQUIRE(true);*/
    /*}*/
}
