#include "CPU.hpp"
#include "DebugTypes.hpp"
#include "InterruptController.hpp"
#include "utils.hpp"

CPU::CPU(InterruptController& ic, Memory& mmu)
    : ic_(ic)
    , memory_(mmu)
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
    , FlagC_(Flag(F_, 4)) {
    reset();
}

void CPU::reset() {
    cycles_ = 0;

    // registers
    AF_.setVal(0x01B0);
    BC_.setVal(0x0013);
    DE_.setVal(0x00D8);
    HL_.setVal(0x014D);
    SP_.setVal(0xFFFE);
    PC_.setVal(0x0100);

    // interrupt flags registers
    IME_ = false;

    // helper flags
    isStopped_ = false;
    isHalted_ = false;
    isHaltBug_ = false;
    is2xSpeed_ = false;
    isEISet_ = false;
    isDISet_ = false;
    isCondMet_ = true;
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

bool CPU::executeInterrupt(u8 i) {
    isHalted_ = false;
    isStopped_ = false;

    if(not IME_)
        return false;

    memory_.write(PC_.hi_, --SP_);
    memory_.write(PC_.lo_, --SP_);
    PC_.hi_ = 0;
    PC_.lo_ = 0x40 + i * 8;

    ic_.disableInterrupt(i);
    IME_ = false;

    cycles_ += 5;
    return true;
}

bool CPU::handleInterrupts() {
    u8 enable = ic_.getIE();
    u8 flag = ic_.getIF();
    u8 req = enable & flag;
    bool serviced = false;
    if(Utils::getBit(req, 0)) {
        serviced = executeInterrupt(0);
    }
    if(Utils::getBit(req, 1)) {
        serviced = executeInterrupt(2);
    }
    if(Utils::getBit(req, 3)) {
        serviced = executeInterrupt(3);
    }
    if(Utils::getBit(req, 4)) {
        serviced = executeInterrupt(4);
    }
    return serviced;
}

CPUDump CPU::getDebugDump() {
    return CPUDump(
        std::array<u8, 4> { fetch8(PC_), fetch8(PC_ + 1), fetch8(PC_ + 2), fetch8(PC_ + 3) },
        AF_.hi_,
        AF_.lo_,
        BC_.hi_,
        BC_.lo_,
        DE_.hi_,
        DE_.lo_,
        HL_.hi_,
        HL_.lo_,
        SP_,
        PC_,
        IME_
    );
}

void CPU::step() {
    if(handleInterrupts())
        return;
    handleIME();

    isCondMet_ = true;
    incrementPC_ = true;

    if(isHalted_)  {
        cycles_ += 4;
        return;
    }

    int opcode = fetch8(PC_);

    // deduce from which map to pick
    Instruction instr;
    if(opcode == 0xCB) {
        opcode = fetch8(PC_+1);
        instr = prefInstrArray_[opcode];
    }
    else {
        instr = unprefInstrArray_[opcode];
    }

    instr.proc_();

    // set flags
    handleFlags(instr.info_.getFlags());

    // add instruction length (in bytes) to program counter
    if(incrementPC_)
        PC_ += instr.info_.getBytes();

    // determine number of cycles that went by while executing instruction
    auto instrCycles = instr.info_.getCycles();
    
    if(isCondMet_)
        cycles_ += instrCycles.first;
    else
        cycles_ += instrCycles.second;
}

void CPU::handleFlags(const Utils::flagArray& flags) {
    FlagZ_.handle(flags[0]);
    FlagN_.handle(flags[1]);
    FlagH_.handle(flags[2]);
    FlagC_.handle(flags[3]);
}

void CPU::handleIME() {
    if(isEISet_) {
        IME_ = true;
        isEISet_ = false;
    }
    else if(isDISet_) {
        IME_ = false;
        isDISet_ = false;
    }
}
