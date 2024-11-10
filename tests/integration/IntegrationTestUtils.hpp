#pragma once

#include <array>

namespace TestUtils 
{
namespace TestNames 
{
namespace CpuInstrs 
{
    std::string main { "cpu_instrs" };
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
} // CpuInstrs
} // TestNames
} // TestUtils
