#pragma once

#include "InterruptController.hpp"
#include "utils.hpp"
#include <vector>

class SerialDataTransfer {
public:
    SerialDataTransfer(InterruptController& ic);

    void writeToTestOutput(u8 controlData);
    void reset();
    std::vector<char> getTestOutput();

    void setSB(u8);
    u8 getSB();
    void setSC(u8);
    u8 getSC();


protected:
    u8 SB_; // serial transfer data
    u8 SC_; // serial control
    std::vector<char> testOutput_;
    InterruptController& ic_;
};
