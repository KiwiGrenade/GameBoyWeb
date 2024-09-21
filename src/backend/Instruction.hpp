#pragma once

#include "utils.hpp"
#include <functional>

struct Instruction {
    struct Info {
        Info() = default;
        Info(const QJsonObject& obj)
        : bytes_(Utils::getBytesFromJsonObject(obj))
        , cycles_(Utils::getCyclesFromJsonObject(obj))
        , flags_(Utils::getFlagsFromJsonObject(obj)) {
        }

        inline u8 getBytes() { return bytes_; }
        inline std::pair<u8, u8> getCycles() { return cycles_; }
        inline Utils::flagArray getFlags() { return flags_; }

    private:
        u8 bytes_;
        std::pair<u8, u8> cycles_;
        Utils::flagArray flags_;
    };

    std::function<bool()> instr_;
    Info info_;
};
