#include "CPU.hpp"

#include <catch2/catch_test_macros.hpp>

struct ProceduresUnprefixedTests : CPU {
    ProceduresUnprefixedTests() : CPU(memory) {
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
    u16 oldSP = SP_;
    u16 oldPC = PC_;
    bool oldFlagZ = FlagZ_.val();
    bool oldFlagN = FlagN_.val();
    bool oldFlagH = FlagH_.val();
    bool oldFlagC = FlagC_.val();

    /*SECTION("0x01") {*/
    /*    execute(0x01);*/
    /*    REQUIRE(true);*/
    /*}*/
    /*SECTION("0x02") {*/
    /*    step();*/
    /*    REQUIRE(true);*/
    /*}*/
    /*SECTION("0x03") {*/
    /*    step();*/
    /*    REQUIRE(true);*/
    /*}*/
    /*SECTION("0x04") {*/
    /*    step();*/
    /*    REQUIRE(true);*/
    /*}*/
    /*SECTION("0x05") {*/
    /*    step();*/
    /*    REQUIRE(true);*/
    /*}*/
    /*SECTION("0x06") {*/
    /*    step();*/
    /*    REQUIRE(true);*/
    /*}*/
    /*SECTION("0x07") {*/
    /*    step();*/
    /*    REQUIRE(true);*/
    /*}*/
    /*SECTION("0x08") {*/
    /*    step();*/
    /*    REQUIRE(true);*/
    /*}*/
    /*SECTION("0x09") {*/
    /*    step();*/
    /*    REQUIRE(true);*/
    /*}*/
    /*SECTION("0x0A") {*/
    /*    step();*/
    /*    REQUIRE(true);*/
    /*}*/
    /*SECTION("0x0B") {*/
    /*    step();*/
    /*    REQUIRE(true);*/
    /*}*/
    /*SECTION("0x0C") {*/
    /*    step();*/
    /*    REQUIRE(true);*/
    /*}*/
    /*SECTION("0x0D") {*/
    /*    step();*/
    /*    REQUIRE(true);*/
    /*}*/
    /*SECTION("0x0E") {*/
    /*    step();*/
    /*    REQUIRE(true);*/
    /*}*/
    /*SECTION("0x0F") {*/
    /*    step();*/
    /*    REQUIRE(true);*/
    /*}*/
    SECTION("0x10", "[STOP]") {
        execute(0x10);
        REQUIRE(isStopped_);
    }
    /*SECTION("0x11") {*/
    /*    step();*/
    /*    REQUIRE(true);*/
    /*}*/
    /*SECTION("0x12") {*/
    /*    step();*/
    /*    REQUIRE(true);*/
    /*}*/
    /*SECTION("0x13") {*/
    /*    step();*/
    /*    REQUIRE(true);*/
    /*}*/
    /*SECTION("0x14") {*/
    /*    step();*/
    /*    REQUIRE(true);*/
    /*}*/
    /*SECTION("0x15") {*/
    /*    step();*/
    /*    REQUIRE(true);*/
    /*}*/
    /*SECTION("0x16") {*/
    /*    step();*/
    /*    REQUIRE(true);*/
    /*}*/
    /*SECTION("0x17") {*/
    /*    step();*/
    /*    REQUIRE(true);*/
    /*}*/
    /*SECTION("0x18") {*/
    /*    step();*/
    /*    REQUIRE(true);*/
    /*}*/
    /*SECTION("0x19") {*/
    /*    step();*/
    /*    REQUIRE(true);*/
    /*}*/
    /*SECTION("0x1A") {*/
    /*    step();*/
    /*    REQUIRE(true);*/
    /*}*/
    /*SECTION("0x1B") {*/
    /*    step();*/
    /*    REQUIRE(true);*/
    /*}*/
    /*SECTION("0x1C") {*/
    /*    step();*/
    /*    REQUIRE(true);*/
    /*}*/
    /*SECTION("0x1D") {*/
    /*    step();*/
    /*    REQUIRE(true);*/
    /*}*/
    /*SECTION("0x1E") {*/
    /*    step();*/
    /*    REQUIRE(true);*/
    /*}*/
    /*SECTION("0x1F") {*/
    /*    step();*/
    /*    REQUIRE(true);*/
    /*}*/
    /*SECTION("0x20") {*/
    /*    step();*/
    /*    REQUIRE(true);*/
    /*}*/
    /*SECTION("0x21") {*/
    /*    step();*/
    /*    REQUIRE(true);*/
    /*}*/
    /*SECTION("0x22") {*/
    /*    step();*/
    /*    REQUIRE(true);*/
    /*}*/
    /*SECTION("0x23") {*/
    /*    step();*/
    /*    REQUIRE(true);*/
    /*}*/
    /*SECTION("0x24") {*/
    /*    step();*/
    /*    REQUIRE(true);*/
    /*}*/
    /*SECTION("0x25") {*/
    /*    step();*/
    /*    REQUIRE(true);*/
    /*}*/
    /*SECTION("0x26") {*/
    /*    step();*/
    /*    REQUIRE(true);*/
    /*}*/
    SECTION("0x27", "[DAA]") {
        FlagC_.clear();
        A_ = 0x9B;
        execute(0x27);
        REQUIRE(FlagC_.val());
        REQUIRE(A_ == 1);
    }
    /*SECTION("0x28") {*/
    /*    step();*/
    /*    REQUIRE(true);*/
    /*}*/
    /*SECTION("0x29") {*/
    /*    step();*/
    /*    REQUIRE(true);*/
    /*}*/
    /*SECTION("0x2A") {*/
    /*    step();*/
    /*    REQUIRE(true);*/
    /*}*/
    /*SECTION("0x2B") {*/
    /*    step();*/
    /*    REQUIRE(true);*/
    /*}*/
    /*SECTION("0x2C") {*/
    /*    step();*/
    /*    REQUIRE(true);*/
    /*}*/
    /*SECTION("0x2D") {*/
    /*    step();*/
    /*    REQUIRE(true);*/
    /*}*/
    /*SECTION("0x2E") {*/
    /*    step();*/
    /*    REQUIRE(true);*/
    /*}*/
    SECTION("0x2F", "[CPL]") {
        A_ = u8(0b11010100);
        execute(0x2f);
        REQUIRE(A_ == u8(0b00101011));
    }
    /*SECTION("0x30") {*/
    /*    step();*/
    /*    REQUIRE(true);*/
    /*}*/
    /*SECTION("0x31") {*/
    /*    step();*/
    /*    REQUIRE(true);*/
    /*}*/
    /*SECTION("0x32") {*/
    /*    step();*/
    /*    REQUIRE(true);*/
    /*}*/
    /*SECTION("0x33") {*/
    /*    step();*/
    /*    REQUIRE(true);*/
    /*}*/
    /*SECTION("0x34") {*/
    /*    step();*/
    /*    REQUIRE(true);*/
    /*}*/
    /*SECTION("0x35") {*/
    /*    step();*/
    /*    REQUIRE(true);*/
    /*}*/
    /*SECTION("0x36") {*/
    /*    step();*/
    /*    REQUIRE(true);*/
    /*}*/
    SECTION("0x37", "[SCF]") {
        execute(0x37);
        REQUIRE_FALSE(FlagN_.val());
        REQUIRE_FALSE(FlagH_.val());
        REQUIRE(FlagC_.val());
    }
    /*SECTION("0x38") {*/
    /*    step();*/
    /*    REQUIRE(true);*/
    /*}*/
    /*SECTION("0x39") {*/
    /*    step();*/
    /*    REQUIRE(true);*/
    /*}*/
    /*SECTION("0x3A") {*/
    /*    step();*/
    /*    REQUIRE(true);*/
    /*}*/
    /*SECTION("0x3B") {*/
    /*    step();*/
    /*    REQUIRE(true);*/
    /*}*/
    /*SECTION("0x3C") {*/
    /*    step();*/
    /*    REQUIRE(true);*/
    /*}*/
    /*SECTION("0x3D") {*/
    /*    step();*/
    /*    REQUIRE(true);*/
    /*}*/
    /*SECTION("0x3E") {*/
    /*    step();*/
    /*    REQUIRE(true);*/
    /*}*/
    SECTION("0x3F", "[CCF]") {
        execute(0x3f);
        REQUIRE(oldFlagC != FlagC_.val());
        execute(0x3f);
        REQUIRE(oldFlagC == FlagC_.val());
    }
    SECTION("0x40-0x45, 0x47-0x4D, 0x4F-0x55, 0x57-0x5D, 0x5F-0x65, 0x67-0x6D, 0x6F, 0x78-0x7D, 0x7F", "[LD]") {
        std::vector<u8*> reg {&B_, &C_, &D_, &E_, &H_, &L_, &A_};
        u16 k = 0x40;
        for(u8 i = 0; i < 8; i++, k += 8) {
            if(k == 0x70)
                continue;
            for(u8 j = 0; j < 8; j++) {
                if(j == 6) 
                    continue;
                u16 op = k + j;
                u8* to = i == 7 ? reg[i-1] : reg[i];
                u8* from = j == 7 ? reg[j-1] : reg[j];
                *from = op;
                execute(op);
                REQUIRE(*to == *from);
            }
        }
    }
    SECTION("0x70-0x75, 0x77", "[LDD]") {
         std::vector<u8*> reg {&B_, &C_, &D_, &E_, &H_, &L_, &A_};
        u16 k = 0x70;
        for(u8 i = 0; i < 8; i++) {
            if(i == 6)
                continue;
            u16 op = k + i;
            u8* from = i == 7 ? reg[i-1] : reg[i];
            *from = op;
            HL_.setVal(op);
            execute(op);
            REQUIRE(memory_.read(HL_) == *from);
        }
        REQUIRE(memory_.read(0x77) == 0x77);
    }
    /*SECTION("0x46") {*/
    /*    step();*/
    /*    REQUIRE(true);*/
    /*}*/
    /*SECTION("0x4E") {*/
    /*    step();*/
    /*    REQUIRE(true);*/
    /*}*/
    /*SECTION("0x56") {*/
    /*    step();*/
    /*    REQUIRE(true);*/
    /*}*/
    /*SECTION("0x5E") {*/
    /*    step();*/
    /*    REQUIRE(true);*/
    /*}*/
    /*SECTION("0x66") {*/
    /*    step();*/
    /*    REQUIRE(true);*/
    /*}*/
    /*SECTION("0x6E") {*/
    /*    step();*/
    /*    REQUIRE(true);*/
    /*}*/
    /*SECTION("0x70") {*/
    /*    step();*/
    /*    REQUIRE(true);*/
    /*}*/
    /*SECTION("0x71") {*/
    /*    step();*/
    /*    REQUIRE(true);*/
    /*}*/
    /*SECTION("0x72") {*/
    /*    step();*/
    /*    REQUIRE(true);*/
    /*}*/
    /*SECTION("0x73") {*/
    /*    step();*/
    /*    REQUIRE(true);*/
    /*}*/
    /*SECTION("0x74") {*/
    /*    step();*/
    /*    REQUIRE(true);*/
    /*}*/
    /*SECTION("0x75") {*/
    /*    step();*/
    /*    REQUIRE(true);*/
    /*}*/
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
    /*SECTION("0x7E") {*/
    /*    step();*/
    /*    REQUIRE(true);*/
    /*}*/
    /*SECTION("0x80") {*/
    /*    step();*/
    /*    REQUIRE(true);*/
    /*}*/
    /*SECTION("0x81") {*/
    /*    step();*/
    /*    REQUIRE(true);*/
    /*}*/
    /*SECTION("0x82") {*/
    /*    step();*/
    /*    REQUIRE(true);*/
    /*}*/
    /*SECTION("0x83") {*/
    /*    step();*/
    /*    REQUIRE(true);*/
    /*}*/
    /*SECTION("0x84") {*/
    /*    step();*/
    /*    REQUIRE(true);*/
    /*}*/
    /*SECTION("0x85") {*/
    /*    step();*/
    /*    REQUIRE(true);*/
    /*}*/
    /*SECTION("0x86") {*/
    /*    step();*/
    /*    REQUIRE(true);*/
    /*}*/
    /*SECTION("0x87") {*/
    /*    step();*/
    /*    REQUIRE(true);*/
    /*}*/
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
    /*SECTION("0xC0") {*/
    /*    step();*/
    /*    REQUIRE(true);*/
    /*}*/
    /*SECTION("0xC1") {*/
    /*    step();*/
    /*    REQUIRE(true);*/
    /*}*/
    /*SECTION("0xC2") {*/
    /*    step();*/
    /*    REQUIRE(true);*/
    /*}*/
    /*SECTION("0xC3") {*/
    /*    step();*/
    /*    REQUIRE(true);*/
    /*}*/
    /*SECTION("0xC4") {*/
    /*    step();*/
    /*    REQUIRE(true);*/
    /*}*/
    /*SECTION("0xC5") {*/
    /*    step();*/
    /*    REQUIRE(true);*/
    /*}*/
    /*SECTION("0xC6") {*/
    /*    step();*/
    /*    REQUIRE(true);*/
    /*}*/
    /*SECTION("0xC7") {*/
    /*    step();*/
    /*    REQUIRE(true);*/
    /*}*/
    /*SECTION("0xC8") {*/
    /*    step();*/
    /*    REQUIRE(true);*/
    /*}*/
    /*SECTION("0xC9") {*/
    /*    step();*/
    /*    REQUIRE(true);*/
    /*}*/
    /*SECTION("0xCA") {*/
    /*    step();*/
    /*    REQUIRE(true);*/
    /*}*/
    /*SECTION("0xCB") {*/
    /*    step();*/
    /*    REQUIRE(true);*/
    /*}*/
    /*SECTION("0xCC") {*/
    /*    step();*/
    /*    REQUIRE(true);*/
    /*}*/
    /*SECTION("0xCD") {*/
    /*    step();*/
    /*    REQUIRE(true);*/
    /*}*/
    /*SECTION("0xCE") {*/
    /*    step();*/
    /*    REQUIRE(true);*/
    /*}*/
    /*SECTION("0xCF") {*/
    /*    step();*/
    /*    REQUIRE(true);*/
    /*}*/
    /*SECTION("0xD0") {*/
    /*    step();*/
    /*    REQUIRE(true);*/
    /*}*/
    /*SECTION("0xD1") {*/
    /*    step();*/
    /*    REQUIRE(true);*/
    /*}*/
    /*SECTION("0xD2") {*/
    /*    step();*/
    /*    REQUIRE(true);*/
    /*}*/
    /*SECTION("0xD3") {*/
    /*    step();*/
    /*    REQUIRE(true);*/
    /*}*/
    /*SECTION("0xD4") {*/
    /*    step();*/
    /*    REQUIRE(true);*/
    /*}*/
    /*SECTION("0xD5") {*/
    /*    step();*/
    /*    REQUIRE(true);*/
    /*}*/
    /*SECTION("0xD6") {*/
    /*    step();*/
    /*    REQUIRE(true);*/
    /*}*/
    /*SECTION("0xD7") {*/
    /*    step();*/
    /*    REQUIRE(true);*/
    /*}*/
    /*SECTION("0xD8") {*/
    /*    step();*/
    /*    REQUIRE(true);*/
    /*}*/
    /*SECTION("0xD9") {*/
    /*    step();*/
    /*    REQUIRE(true);*/
    /*}*/
    /*SECTION("0xDA") {*/
    /*    step();*/
    /*    REQUIRE(true);*/
    /*}*/
    /*SECTION("0xDB") {*/
    /*    step();*/
    /*    REQUIRE(true);*/
    /*}*/
    /*SECTION("0xDC") {*/
    /*    step();*/
    /*    REQUIRE(true);*/
    /*}*/
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
    /*SECTION("0xE0") {*/
    /*    step();*/
    /*    REQUIRE(true);*/
    /*}*/
    /*SECTION("0xE1") {*/
    /*    step();*/
    /*    REQUIRE(true);*/
    /*}*/
    /*SECTION("0xE2") {*/
    /*    step();*/
    /*    REQUIRE(true);*/
    /*}*/
    /*SECTION("0xE3") {*/
    /*    step();*/
    /*    REQUIRE(true);*/
    /*}*/
    /*SECTION("0xE4") {*/
    /*    step();*/
    /*    REQUIRE(true);*/
    /*}*/
    /*SECTION("0xE5") {*/
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
    /*SECTION("0xE9") {*/
    /*    step();*/
    /*    REQUIRE(true);*/
    /*}*/
    /*SECTION("0xEA") {*/
    /*    step();*/
    /*    REQUIRE(true);*/
    /*}*/
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
    /*SECTION("0xF0") {*/
    /*    step();*/
    /*    REQUIRE(true);*/
    /*}*/
    /*SECTION("0xF1") {*/
    /*    step();*/
    /*    REQUIRE(true);*/
    /*}*/
    /*SECTION("0xF2") {*/
    /*    step();*/
    /*    REQUIRE(true);*/
    /*}*/
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
    /*SECTION("0xF5") {*/
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
    /*SECTION("0xF8") {*/
    /*    step();*/
    /*    REQUIRE(true);*/
    /*}*/
    /*SECTION("0xF9") {*/
    /*    step();*/
    /*    REQUIRE(true);*/
    /*}*/
    /*SECTION("0xFA") {*/
    /*    step();*/
    /*    REQUIRE(true);*/
    /*}*/
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
