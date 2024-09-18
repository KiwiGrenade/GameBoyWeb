#pragma once


#include <cstdint>

#include "utils.hpp"
#include "Memory.hpp"

#include <QPlainTextEdit>

class Disassembler {
public:
    Disassembler();
    ~Disassembler() = default;

    void disassemble(const Memory& memory, QPlainTextEdit& plainTextEdit);

private:
    
    Utils::Assembly disassembleOneOpcode(const uint8_t& byte);
    QJsonObject getJsonObjectFromFile(std::string& filePath);
    bool prefix_;
};


