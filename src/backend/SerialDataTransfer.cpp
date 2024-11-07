#include "SerialDataTransfer.hpp"

SerialDataTransfer::SerialDataTransfer(InterruptController& ic) 
    : ic_(ic) {
}

void SerialDataTransfer::writeToTestOutput(u8 controlData) {
    testOutput_.push_back((char)SB_);
    ic_.requestInterrupt(InterruptController::Type::Serial);
}

std::vector<char> SerialDataTransfer::getTestOutput() {
    return testOutput_;
}

void SerialDataTransfer::reset() {
    SB_ = 0;
    SC_ = 0;
}

void SerialDataTransfer::setSB(u8 byte) {
    SB_ = byte;
}

u8 SerialDataTransfer::getSB() {
    return SB_;
}

void SerialDataTransfer::setSC(u8 byte) {
    SC_ = byte;
    if(byte == 0x81)
        writeToTestOutput(byte);
}

u8 SerialDataTransfer::getSC() {
    return SC_;
}