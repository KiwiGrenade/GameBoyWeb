#include "GameBoy.hpp"
#include <memory>

GameBoy::GameBoy()
    : memory_(std::make_shared<Memory>())
    , cpu_(std::make_shared<CPU>(*memory_)){
}
