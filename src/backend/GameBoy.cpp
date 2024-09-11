#include "GameBoy.hpp"

GameBoy::GameBoy()
    : memory_{std::make_shared<Memory>()} {

}
