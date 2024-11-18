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
typedef uint16_t u16;

#define U16_MAX UINT16_MAX 

namespace Utils {

const std::string YELLOW = "\e[36m";
const std::string RED = "\e[31m";
const std::string END = "\e[0m";

inline void warning(const std::string& str) { std::cerr << YELLOW << "[WRN]: " << str << END << std::endl; }
inline void error(const std::string& str) { std::cerr << RED << "[ERR]: " << str << END << std::endl; exit(2); }

static std::string jsonFilePath = "/home/desktop/GameBoyWeb/res/opcodes.json";
static std::string romsPath = "/home/jaskow/GameBoyWeb/roms";

// bits
inline static u8 setBit(u8& number, const u8 n, const bool x) {
    return number = (number & ~((u8)1 << n)) | ((u8)x << n);
}

inline static bool getBit(const u8& byte, const u8 n) {
    return (byte >> n) & (u8)1;
}

} // Utils

