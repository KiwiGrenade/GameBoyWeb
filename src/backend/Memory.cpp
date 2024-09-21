#include "Memory.hpp"

#include <iostream>

Memory::Memory()
    : memory_(std::make_shared<std::array<uint8_t, size_>>()) {
}

void Memory::load(const QByteArray& rom) {
    // omit /0 at end of QByteArray
    // TODO: FIX THIS
    std::cout << rom.size() << std::endl;
    for(size_t i = 0; i < rom.size()-1; i++) {
        write(rom[i], i);
    }
}

