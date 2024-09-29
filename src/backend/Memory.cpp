#include "Memory.hpp"

Memory::Memory() {
}

void Memory::loadSimpleROM(const QByteArray& rom) {
    // omit /0 at end of QByteArray
    // TODO: FIX THIS
    for(u16 i = 0; i < 0x7FFF; i++) {
        memory_[i] = rom[i];
    }
}

