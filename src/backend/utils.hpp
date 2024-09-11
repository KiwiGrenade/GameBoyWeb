#pragma once

#include <iostream>
#include <cfloat>
/*#include <cstdint>*/
/*#include <memory>*/
#include <string>

const std::string YELLOW = "\e[36m";
const std::string RED = "\e[31m";
const std::string END = "\e[0m";

inline void warning(const std::string& str) { std::cerr << YELLOW << "[WRN]: " << str << END << std::endl; }
inline void error(const std::string& str) { std::cerr << RED << "[ERR]: " << str << END << std::endl; exit(2); }


/*enum Flag {*/
/*    zero,*/
/*    one,*/
/*    flag,*/
/*    none*/
/*};*/
/**/
/*struct Assembly {*/
/*    std::string mnemonic;*/
/*    std::shared_ptr<void> arg1;*/
/*    std::shared_ptr<void> arg2;*/
/**/
/*    uint16_t length;*/
/*    uint16_t duration;*/
/*    Flag Z;*/
/*    Flag N;*/
/*    Flag H;*/
/*    Flag C;*/
/*};*/

