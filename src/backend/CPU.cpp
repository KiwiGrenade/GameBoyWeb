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
    , IE_(memory_.getIE())
    , IF_(memory_.getIF()) {
    reset();
}

void CPU::reset() {
    AF_.setVal(0x11B0);
    BC_.setVal(0x0013);
    DE_.setVal(0x00D8);
    HL_.setVal(0x014D);
    SP_.setVal(0xFFFE);
    PC_.setVal(0x1000);
    cycles_ = 0;
    IME_ = false;
    // helper flags
    isStopped_ = false;
    isHalted_ = false;
    isHaltBug_ = false;
    is2xSpeed_ = false;
    isEISet_ = false;
    isDISet_ = false;
    isCondMet_ = false;
    incrementPC_ = true;
}

InstrArray CPU::getInstrArray(const bool prefixed) {
    QJsonObject instrMapJsonObj = Utils::getInstrMapJsonObjectFromFile(prefixed);
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
    handleIME();
    if(isHalted_) {
        // assume nop
        cycles_ += 4;
        return;
    }
    isCondMet_ = false;
    incrementPC_ = true;

    u8 opcode = memory_.read(PC_);
    
    // deduce from which map to pick
    Instruction instr = 
        isPrefixed_ ? prefInstrArray_[opcode] : unprefInstrArray_[opcode];

    instr.proc_();

    // set flags
    handleFlags(instr.info_.getFlags());

    // add instruction length (in bytes) to program counter
    if(incrementPC_)
        PC_ += instr.info_.getBytes();

    // determine number of cycles that went by while executing instruction
    std::pair <u8, u8> cycles = instr.info_.getCycles();
    cycles_ += isCondMet_ ? cycles.first : cycles.second;
}

void CPU::handleFlags(const Utils::flagArray& flags) {
    FlagZ_.handle(flags[0]);
    FlagN_.handle(flags[1]);
    FlagH_.handle(flags[2]);
    FlagC_.handle(flags[3]);
}

void CPU::handleIME() {
    if(isEISet_)
        IME_ = true;
    else if(isDISet_)
        IME_ = false;
}
