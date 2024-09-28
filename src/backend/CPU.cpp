#include "CPU.hpp"
#include "utils.hpp"

CPU::CPU(Memory& memory)
    : memory_(memory)
    , prefInstrArray_(getInstrArray(true))
    , unprefInstrArray_(getInstrArray(false))
    , A_(AF_.hi_)
    , F_(AF_.lo_)
    , B_(BC_.hi_)
    , C_(BC_.lo_)
    , D_(DE_.hi_)
    , E_(DE_.lo_)
    , H_(HL_.hi_)
    , L_(HL_.lo_)
    , FlagZ_(Flag(F_, 7))
    , FlagN_(Flag(F_, 6))
    , FlagH_(Flag(F_, 5))
    , FlagC_(Flag(F_, 4))
    , PC_(0)
    , cycles_(0){
}

InstrArray CPU::getInstrArray(const bool prefixed) {
    QJsonObject instrMapJsonObj = Utils::getInstrMapJsonObjectFromFile(Utils::jsonFilePath, prefixed);
    ProcArray procArray = prefixed ? getPrefProcArray() : getUnprefProcArray(); 
    
    InstrArray instrArray;

    // add instruction info to maps
    for(u16 op = 0; op < 256; ++op) {
        QString key = instrMapJsonObj.keys().at(op);
        QJsonObject opcodeJsonObj = instrMapJsonObj.value(key).toObject();

        instrArray[op] = Instruction(opcodeJsonObj, procArray[op]);
    }

    return instrArray;
}

void CPU::step() {
    u8 opcode = memory_.read(PC_);
    
    // deduce from which map to pick
    Instruction instr = 
        isPrefixed_ ? prefInstrArray_[opcode] : unprefInstrArray_[opcode];

    bool cond = instr.proc_();

    // set flags
    handleFlags(instr.info_.getFlags());

    // add instruction length (in bytes) to program counter
    PC_ += instr.info_.getBytes();

    // determine number of cycles that went by while executing instruction
    std::pair <u8, u8> cycles = instr.info_.getCycles();
    cycles_ += cond ? cycles.first : cycles.second;
}

void CPU::handleFlags(const Utils::flagArray& flags) {
    FlagZ_.handle(flags[0]);
    FlagN_.handle(flags[1]);
    FlagH_.handle(flags[2]);
    FlagC_.handle(flags[3]);
}
