#include "GameBoy.hpp"

int main() {
    auto car  = std::make_shared<Cartridge>("/home/jaskow/Repo/GameBoyWeb/roms/tests/Blaarg/cpu_instrs/individual/01-special.gb");
    GameBoy gb;
    gb.loadCartridge(car);

    for(uint32_t i = 0; i < 40000; ++i) {
        std::cout << gb.getCPUDebugDump() << std::endl;
        gb.emulateStep();
    }
    return 0;
}
