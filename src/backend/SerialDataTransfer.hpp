#pragma once

#include "utils.hpp"
#include "InterruptController.hpp"
#include <vector>

class SerialDataTransfer {
public:
    SerialDataTransfer(InterruptController &ic);

    void writeToTestOutput();
    void reset();
    std::vector<char> getTestOutput();
    void setSB(u8);
    u8 getSB();
    void setSC(u8);
    u8 getSC();


protected:
    InterruptController &ic_;
    /*InterruptController& ic_;*/
    std::vector<char> testOutput_;
    u8 SB_; // serial transfer data
    u8 SC_; // serial control
};
