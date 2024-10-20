#include "Instruction.hpp"

Instruction::Instruction(const QJsonObject& obj)
    : info_(Info(obj)) {
}

    // should there be reference to a function or just function object?
Instruction::Instruction(const QJsonObject& obj, std::function<void()> func)
    : info_(Info(obj))
    , proc_(func) {
}

Instruction::Info::Info(const QJsonObject& obj)
    : bytes_(Utils::getBytesFromInstrJsonObject(obj))
    , cycles_(Utils::getCyclesFromInstrJsonObject(obj))
    , flags_(Utils::getFlagsFromInstrJsonObject(obj)) {
}

