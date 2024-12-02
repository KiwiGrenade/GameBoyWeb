#include "GameBoy.hpp"
#include "IntegrationTestUtils.hpp"
#include <memory>
#include <string>

using namespace TestUtils::TestNames;

int main(int argc, char *argv[]) {
    int numberOfCycles = std::stoi(argv[2]);
    int testNumber = std::stoi(argv[1]) - 1;
    std::string testName { CpuInstrs::individual[testNumber]};
    std::string testRomPath = Utils::romsPath + "/tests/Blaarg/cpu_instrs/individual/" + testName + ".gb";

    auto car  = std::make_shared<Cartridge>(testRomPath);
    auto gb = std::make_shared<GameBoy>();

    gb->loadCartridge(car);

    for(int i = 0; i < numberOfCycles; ++i) {
        std::cout << gb->getCPUDebugDump() << std::endl;
        gb->step();
    }
    /*std::cout << gb->getSerialOutput() << std::endl;*/
    return 0;
}
