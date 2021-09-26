/*
 * Main file for the spaceInvaders emulator program.
 * This main file is for the SFML library
 */

#include <iostream>
#include <cstdio>
#include "i8080.h"
#include "io_devices.h"
#include "platform.h"

int main(int argc, char** argv)
{
    std::cout << "Space Invaders Emulator!" << std::endl;

    // Spawns
    Io_devices devices;
    CPU_8080 cpu;
    PLATFORMTYPE platform;

    // connections
    devices.Connect(&platform);
    cpu.Connect_io_dev(&devices);

    // Setup io devices
    devices.Initialize();

    // Setup cpu
    std::cout << "Setup CPU..." << std::endl;
    cpu.Initialize();
    cpu.LoadRom("invaders");

    // Setup System Platform
    platform.Initialize(&devices, &cpu);

    platform.BeginEmulation();

    return 0;
}
