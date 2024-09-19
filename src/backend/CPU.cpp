#include "CPU.hpp"

CPU::CPU()
    : A_(AF_.hi_)
    , F_(AF_.lo_)
    , B_(BC_.hi_)
    , C_(BC_.lo_)
    , D_(DE_.hi_)
    , E_(DE_.lo_)
    , H_(HL_.hi_)
    , L_(HL_.lo_) {
}
