/*
 * Main file for the spaceInvaders emulator program.
 * This main file is for the SFML library
 */

#include <iostream>
#include <cstdio>
#include "i8080.h"
#include "io_devices.h"
#include "platform.h"
#include "memory.h"

int main(int argc, char** argv)
{
    // Spawn modules
    Io_devices      devices;
    CPU_8080        cpu;
    PLATFORMTYPE    platform;
    Memory          memory;

    // Modules interconnections
    devices.Connect(&platform);
    cpu.Connect(&devices, &memory);

    // Module initializations
    devices.Initialize();
    cpu.Initialize();
    memory.Initialize();
    memory.LoadRom("invaders");
    platform.Initialize(&devices, &cpu);

    // System operation
    platform.BeginEmulation();

    return 0;
}
