#include "GameBoy.hpp"
#include <memory>
#include <string>

std::array<std::string, 11> individual {
    "01-special",
    "02-interrupts",
    "03-op sp,hl",
    "04-op r,imm",
    "05-op rp",
    "06-ld r,r",
    "07-jr,jp,call,ret,rst",
    "08-misc instrs",
    "09-op r,r",
    "10-bit ops",
    "11-op a,(hl)"
};

int main(int argc, char *argv[]) {
    /*int numberOfCycles = std::stoi(argv[2]);*/
    /*int testNumber = std::stoi(argv[1]) - 1;*/
    /*std::string testName { individual[testNumber]};*/


    for(auto testName : individual) {
        /*DYNAMIC_SECTION(testName) {*/
            std::cout << "Test name:" << testName << std::endl;
            std::string testRomPath = Utils::romsPath + "/tests/Blaarg/cpu_instrs/individual/" + testName + ".gb";
            auto car  = std::make_shared<Cartridge>(testRomPath);
            auto gb = std::make_shared<GameBoy>();

            gb->loadCartridge(car);

            for(uint32_t i = 0; i < 8000000; ++i) {
                gb->emulateStep();
            }

            std::string testOutput = gb->getSerialOutput();
            std::cout << "Test output: " << testOutput << std::endl;
    }
    /*std::string testRomPath = Utils::romsPath + "/tests/Blaarg/cpu_instrs/individual/" + testName + ".gb";*/
    /**/
    /*auto car  = std::make_shared<Cartridge>(testRomPath);*/
    /*auto gb = std::make_shared<GameBoy>();*/
    /**/
    /*gb->loadCartridge(car);*/
    /**/
    /*for(int i = 0; i < numberOfCycles; ++i) {*/
    /*    std::cout << gb->getCPUDebugDump() << std::endl;*/
    /*    gb->emulateStep();*/
    /*}*/
    /*std::cout << gb->getSerialOutput() << std::endl;*/
    return 0;
}
