#pragma once

#include "utils.hpp"
#include <functional>
#include <QJsonObject>

struct Instruction {
    Instruction() = default;
    Instruction(const QJsonObject& obj);
    Instruction(const QJsonObject& obj, std::function<bool()> func); // function reference or function object?

    struct Info {
        Info() = default;
        Info(const QJsonObject& obj);

        // getters
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
