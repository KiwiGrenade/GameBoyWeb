#pragma once

#include "utils.hpp"
#include <functional>
#include <QJsonObject>

struct Instruction {

    Instruction() = default;

    Instruction(const QJsonObject& obj)
        : info_(Info(obj)) {
    }

    // should there be reference to a function or just function object?
    Instruction(const QJsonObject& obj, std::function<bool()> func)
        : info_(Info(obj))
        , instr_(func) {
    }

    struct Info {
        Info() = default;
        Info(const QJsonObject& obj)
        : bytes_(Utils::getBytesFromInstrJsonObject(obj))
        , cycles_(Utils::getCyclesFromInstrJsonObject(obj))
        , flags_(Utils::getFlagsFromInstrJsonObject(obj)) {
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
