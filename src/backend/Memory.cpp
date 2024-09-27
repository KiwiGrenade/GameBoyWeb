#include "Memory.hpp"

Memory::Memory()
    : memory_(std::make_shared<std::array<uint8_t, size_>>()) {
}

void Memory::loadSimpleROM(const QByteArray& rom) {
    // omit /0 at end of QByteArray
    // TODO: FIX THIS
    for(u16 i = 0; i < 0x7FFF; i++) {
        memory_->at(i) = rom[i];
    }
}

