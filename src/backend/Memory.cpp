#include "Memory.hpp"

Memory::Memory()
    : memory_(std::make_shared<std::array<uint8_t, size_>>()) {

}

void Memory::load(const QByteArray& rom) {
    // omit /0 at end of QByteArray
    for(size_t i = 0; i < rom.size() - 1; i++) {
        memory_->at(i) = rom[i];
    }
}

