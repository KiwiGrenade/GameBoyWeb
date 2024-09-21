#include "CPU.hpp"

#include <string>
#include <array>

#include <QtLogging>
#include <qjsonobject.h>
#include <qlogging.h>

#include "utils.hpp"
#include "Instruction.hpp"

CPU::CPU(Memory& memory)
    : memory_(memory)
    , A_(AF_.hi_)
    , flags_(AF_.lo_)
    , B_(BC_.hi_)
    , C_(BC_.lo_)
    , D_(DE_.hi_)
    , E_(DE_.lo_)
    , H_(HL_.hi_)
    , L_(HL_.lo_) {
    QJsonObject obj = Utils::getJsonObjectFromFile(Utils::jsonFilePath);
    QJsonObject unprefixed = obj.value("unprefixed").toObject();
    QJsonObject prefixed = obj.value("cbprefixed").toObject();
    
    // add instruction info to maps
    for(QString opcode : unprefixed.keys()) {
        QJsonObject unprefOpcodeJsonObj = unprefixed.value(opcode).toObject();
        QJsonObject prefOpcodeJsonObj = prefixed.value(opcode).toObject();

        unprefInstrInfoMap_.emplace(
            std::make_pair(opcode.toUShort(), Instruction::Info(unprefOpcodeJsonObj))
        );
        prefInstrInfoMap_.emplace(
            std::make_pair(opcode.toUShort(), Instruction::Info(prefOpcodeJsonObj))
        );
    }
}

void CPU::step() {
    u8 opcode = memory_.read(pc_);
    
    // deduce from which map to pick
    Instruction::Info info = 
        isPrefixed_ ? prefInstrInfoMap_[opcode] : unprefInstrInfoMap_[opcode];

    executeOpcode(opcode);

    // set flags
    handleFlags(info.getFlags());

    // add instruction length (in bytes) to program counter
    pc_ += info.getBytes();

    // determine number of cycles that went by while executing instruction
    std::pair <u8, u8> cycles = info.getCycles();
    cycles_ += isCondMet_ ? cycles.first : cycles.second;
}

void CPU::handleFlags(const Utils::flagArray& flags) {
    for(u8 i = 0; i < flags.size(); i++) {
        u8 nBit = 7 - i;
        if (flags[i] == Utils::Flag::set)
            Utils::setBit(flags_, nBit);
        else if (flags[i] == Utils::Flag::reset)
            Utils::resetBit(flags_, nBit);
    }
}


