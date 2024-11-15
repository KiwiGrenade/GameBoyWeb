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

static std::string jsonFilePath = "/home/jaskow/GameBoyWeb/res/opcodes.json";
static std::string romsPath = "/home/jaskow/GameBoyWeb/roms";

enum class Flag : char {
    reset,
    set,
    nothing,
    setOrReset
};

typedef std::array<Flag, 4> flagArray;

// std::string uint8_tToHexString(const uint8_t& byte);

// json object extraction
QJsonObject getInstrMapJsonObjectFromFile(const bool prefixed);
u8 getBytesFromInstrJsonObject(const QJsonObject& obj);
std::pair<u8, u8> getCyclesFromInstrJsonObject(const QJsonObject& obj);
Utils::flagArray getFlagsFromInstrJsonObject(const QJsonObject& obj);

// bits
inline static u8 setBit(u8& number, const u8 n, const bool x) {
    return number = (number & ~((u8)1 << n)) | ((u8)x << n);
}

inline static bool getBit(const u8& byte, const u8 n) {
    return (byte >> n) & (u8)1;
}

} // Utils

