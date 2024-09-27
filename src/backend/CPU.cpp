#include "CPU.hpp"

CPU::CPU(Memory& memory)
    : memory_(memory)
    , prefInstrMap_(getInstrMap(true))
    , unprefInstrMap_(getInstrMap(false))
    , A_(AF_.hi_)
    , flags_(AF_.lo_)
    , B_(BC_.hi_)
    , C_(BC_.lo_)
    , D_(DE_.hi_)
    , E_(DE_.lo_)
    , H_(HL_.hi_)
    , L_(HL_.lo_)
    , pc_(0) {
}

std::unordered_map<u8, Instruction> CPU::getInstrMap(const bool prefixed) {
    QJsonObject instrMapJsonObj = Utils::getInstrMapJsonObjectFromFile(Utils::jsonFilePath, prefixed);
    std::unordered_map<u8, std::function<bool()>> procMap = getProcMap(prefixed); 
    
    std::unordered_map<u8, Instruction> instrMap;

    // add instruction info to maps
    for(QString opcode : instrMapJsonObj.keys()) {
        QJsonObject opcodeJsonObj = instrMapJsonObj.value(opcode).toObject();
        u8 op = opcode.toUShort();
        instrMap.emplace(
            std::make_pair(op, Instruction(opcodeJsonObj, procMap[op]))
        );
    }

    return instrMap;
}

std::unordered_map<u8, std::function<bool()>> CPU::getProcMap(const bool prefixed) {
    std::unordered_map<u8, std::function<bool()>> procMap = {
            {0x00, []{ return false; }}
        };
    return procMap;
}

void CPU::step() {
    u8 opcode = memory_.read(pc_);
    
    // deduce from which map to pick
    Instruction instr = 
        isPrefixed_ ? prefInstrMap_[opcode] : unprefInstrMap_[opcode];

    instr.proc_();

    // set flags
    handleFlags(instr.info_.getFlags());

    // add instruction length (in bytes) to program counter
    pc_ += instr.info_.getBytes();

    // determine number of cycles that went by while executing instruction
    std::pair <u8, u8> cycles = instr.info_.getCycles();
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




