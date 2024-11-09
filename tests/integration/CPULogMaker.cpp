#include "GameBoy.hpp"
#include <memory>

int main() {
    std::string testRomPath = Utils::romsPath + "/tests/Blaarg/cpu_instrs/individual/01-special.gb";
    auto car  = std::make_shared<Cartridge>(testRomPath);
    auto gb = std::make_shared<GameBoy>();
    gb->loadCartridge(car);

    for(uint32_t i = 0; i < 1260000; ++i) {
        std::cout << gb->getCPUDebugDump() << std::endl;
        gb->emulateStep();
    }
    gb->getSerialOutput();
    return 0;
}
