#include "RegisterPair.hpp"

#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators_adapters.hpp>
#include <catch2/generators/catch_generators_random.hpp>


TEST_CASE("constructor", "[RegisterPair]") {
    SECTION("default") {
        RegisterPair reg;

        REQUIRE(reg.hi_ == 0);
        REQUIRE(reg.lo_ == 0);
    }

    u16 val = GENERATE(0, 1, 13, U16_MAX, -1);
    SECTION("u16 operand") {
        RegisterPair reg{val};

        REQUIRE(reg.getVal() == val);
    }
}

TEST_CASE("getters and setters", "[RegisterPair]") {
    u16 val = GENERATE(0, 1, 13, U16_MAX, -1);
    RegisterPair reg1 {val};

    SECTION("setVal and getVal") {
        reg1.setVal(val);

        REQUIRE(reg1.getVal() == val);
    }
}

TEST_CASE("conversions", "[RegisterPair]") {
    u16 val = GENERATE(0, 1, 13, U16_MAX, -1);
    SECTION("explicit") {
        SECTION("u16ToRegisterPair") {
            RegisterPair reg = RegisterPair(val);

            REQUIRE(reg.getVal() == val);
        }

        SECTION("RegisterPairToU16") {
            RegisterPair reg{val};
            u16 val2 = reg;

            REQUIRE(val2 == val);
        }
    }
    SECTION("implicit") {
        SECTION("u16ToRegisterPair") {
            RegisterPair reg = val;

            REQUIRE(reg.getVal() == val);
        }

        SECTION("RegisterPairToU16") {
            RegisterPair reg{val};
            u16 val2 = reg;

            REQUIRE(val2 == val);
        }
    }
}

TEST_CASE("operators", "[RegisterPair]") {
    u16 val1 = GENERATE(0, 1, 13, U16_MAX, -1);
    u16 val2 = GENERATE(0, 20, U16_MAX, -1);
    RegisterPair reg1{val1};
    RegisterPair reg2{val2};


    SECTION("operator=") {
        reg2 = reg1;
        REQUIRE(reg2.getVal() == val1);
    }

    SECTION("operator+=") {
        reg2 += reg1;
        REQUIRE(reg2.getVal() == u16(val2 + val1));
    }

    SECTION("operator-=") {
        reg2 -= reg1;
        REQUIRE(reg2.getVal() == u16(val2 - val1));
    }

    SECTION("prefix operator++") {
        ++reg1;
        ++val1;
        REQUIRE(reg1.getVal() == val1);
    }

    SECTION("postfix operator++") {
        reg2 = reg1;
        reg1++;
        val1++;
        ++reg2;
        REQUIRE(reg1.getVal() == val1);
        REQUIRE(reg2.getVal() == val1);
    }

    SECTION("prefix operator--") {
        --reg1;
        --val1;
        REQUIRE(reg1.getVal() == val1);
    }

    SECTION("postfix operator--") {
        reg2 = reg1;
        reg1--;
        val1--;
        --reg2;
        REQUIRE(reg1.getVal() == val1);
        REQUIRE(reg2.getVal() == val1);
    }

    u16 val3 = GENERATE(take(1000, random(0, U16_MAX)));
    RegisterPair reg3 {val3};
    RegisterPair resReg;
    u16 resU16;
    SECTION("operator+") {
        SECTION("RegisterPair") {
            resReg = reg1 + reg2;
            resU16 = val1 + val2;
            REQUIRE(resReg.getVal() == resU16);

            resReg = reg2 + reg1 + reg3;
            resU16 = val2 + val1 + val3;
            REQUIRE(resReg.getVal() == resU16);
        }
        SECTION("Int") {
            resReg = reg1 + 40;
            resU16 = val1 + 40;
            REQUIRE(resReg.getVal() == resU16);

            resReg = reg1 + (-40);
            resU16 = val1 + (-40);
            REQUIRE(resReg.getVal() == resU16);
        }
    }

    SECTION("operator-") {
        resReg = reg1 - reg2;
        resU16 = val1 - val2;
        REQUIRE(resReg.getVal() == resU16);

        resReg = reg2 - reg1;
        resU16 = val2 - val1;
        REQUIRE(resReg.getVal() == resU16);

        resReg = reg2 - reg1 - reg3;
        resU16 = val2 - val1 - val3;
        REQUIRE(resReg.getVal() == resU16);
    }
}

