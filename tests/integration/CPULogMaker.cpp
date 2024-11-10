#include "GameBoy.hpp"
#include "IntegrationTestUtils.hpp"
#include <memory>
#include <string>

int main(int argc, char *argv[]) {
    std::string input { argv[1] };
    int testNumber = std::stoi(input) - 1;
    std::string testName { TestUtils::blaargTestNames[testNumber]};
    std::string testRomPath = Utils::romsPath + "/tests/Blaarg/cpu_instrs/individual/" + testName + ".gb";

    auto car  = std::make_shared<Cartridge>(testRomPath);
    auto gb = std::make_shared<GameBoy>();

    gb->loadCartridge(car);

    for(uint32_t i = 0; i < 10360000; ++i) {
        std::cout << gb->getCPUDebugDump() << std::endl;
        gb->emulateStep();
    }
    /*std::cout << gb->getSerialOutput() << std::endl;*/
    return 0;
}
