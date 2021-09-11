/*
 * Main file for the spaceInvaders emulator program.
 */

#include <iostream>
#include "i8080.h"

int main(int argc, char** argv) {

    std::cout << "Space Invaders Emulator!" << std::endl;

    // Setup cpu
    std::cout << "Setup CPU..." << std::endl;
    CPU_8080 cpu;
    cpu.Initialize();
    cpu.LoadRom();

    // Emulation cycle
    std::cout << "Starting emulation..." << std::endl;
    while(cpu.Running())
    {
        cpu.PrintState();
        cpu.EmulateCycle();
    }

    cpu.DumpMemory();
    std::cout << "Bye!" << std::endl;

    return 0;
}
