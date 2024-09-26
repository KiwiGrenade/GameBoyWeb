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

static std::string jsonFilePath = "/home/desktop/GameBoyWeb/res/opcodes.json";

enum class Flag : char {
    reset,
    set,
    nothing,
    setOrReset
};

typedef std::array<Flag, 4> flagArray;

std::string uint8_tToHexString(const uint8_t& byte);

// json object extraction
QJsonObject getInstrMapJsonObjectFromFile(const std::string& filePath, const bool prefixed);
u8 getBytesFromInstrJsonObject(const QJsonObject& obj);
std::pair<u8, u8> getCyclesFromInstrJsonObject(const QJsonObject& obj);
Utils::flagArray getFlagsFromInstrJsonObject(const QJsonObject& obj);

// bits
inline static u8 setBit(u8& byte, const u8 n) {
    return byte |= ((u8) 1 << n);
}

inline static u8 resetBit(u8& byte, const u8 n) {
    return byte &= ~((u8) 1 << n);
}

inline static bool getBit(const u8& byte, u8 n) {
    return (byte >> n) & (u8)1;
}

} // Utils

