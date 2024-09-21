#pragma once

#include <iostream>
#include <cfloat>
#include <cstdint>
#include <QObject>
#include <QFile>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonParseError>
#include <string>

typedef uint8_t u8;
typedef uint8_t Register;
typedef uint16_t u16;

#define U16_MAX UINT16_MAX 

namespace Utils {

const std::string YELLOW = "\e[36m";
const std::string RED = "\e[31m";
const std::string END = "\e[0m";

inline void warning(const std::string& str) { std::cerr << YELLOW << "[WRN]: " << str << END << std::endl; }
inline void error(const std::string& str) { std::cerr << RED << "[ERR]: " << str << END << std::endl; exit(2); }

enum Flag : char {
    nothing = '-',
    reset = '0',
    set = '1',
    setOrReset = '2'
};

std::string uint8_tToHexString(const uint8_t& byte);

QJsonObject getJsonObjectFromFile(std::string& filePath);

} // Utils

