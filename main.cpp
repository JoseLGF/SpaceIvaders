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
    cpu.PrintState();

    std::cout << "Bye!" << std::endl;

    return 0;
}
