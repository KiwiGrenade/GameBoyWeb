#include "SerialDataTransfer.hpp"

#include <catch2/catch_test_macros.hpp>

struct SerialDataTransferTests : SerialDataTransfer {
    InterruptController ic = InterruptController();
    SerialDataTransferTests() : SerialDataTransfer(ic) {
    }
};

TEST_CASE_METHOD(SerialDataTransferTests, "constructor") {
    REQUIRE(SB_ == 0);
    REQUIRE(SC_ == 0);
}

TEST_CASE_METHOD(SerialDataTransferTests, "reset") {
    SB_ = 1;
    SC_ = 1;

    reset();

    REQUIRE(SB_ == 0);
    REQUIRE(SC_ == 0);
}

TEST_CASE("OutputFunctionality") {
    InterruptController ic = InterruptController();
    SerialDataTransfer serial = SerialDataTransfer(ic);

    std::vector<char> expVec {'S', 'E', 'R', 'I', 'A', 'L'};
    for(auto c : expVec) {
        serial.setSB((u8) c);
        serial.setSC(0x81);
    }
    REQUIRE(expVec == serial.getTestOutput());
}
