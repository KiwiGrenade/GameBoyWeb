#pragma once

#include <iostream>
#include <cfloat>
#include <cstdint>
#include <string>

const std::string YELLOW = "\e[36m";
const std::string RED = "\e[31m";
const std::string END = "\e[0m";

inline void warning(const std::string& str) { std::cerr << YELLOW << "[WRN]: " << str << END << std::endl; }
inline void error(const std::string& str) { std::cerr << RED << "[ERR]: " << str << END << std::endl; exit(2); }


enum Flag {
    reset,
    set,
    none
};

struct Argument {
    std::string name;
    bool immediate;
};

struct Assembly {
    std::string mnemonic;

    Argument arg1;
    Argument arg2;

    uint16_t length;
    std::pair<uint8_t, uint8_t> duration;

    Flag Z;
    Flag N;
    Flag H;
    Flag C;

    /*std::string toString();*/
};

