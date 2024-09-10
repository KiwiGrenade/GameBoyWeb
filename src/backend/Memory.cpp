#include "Memory.hpp"

Memory::Memory()
    : memory_(std::make_shared<std::array<uint8_t, size_>>())
{
}
