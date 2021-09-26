/*
 * Main file for the spaceInvaders emulator program.
 * This main file is for the SFML library
 */

#ifdef LIB_SFML

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/System.hpp>

#include <iostream>
#include <cstdio>
#include "i8080.h"
#include "io_devices.h"
#include "platform_sfml.h"

int main(int argc, char** argv)
{
    std::cout << "Space Invaders Emulator!" << std::endl;

    // Setup io devices
    Io_devices devices;
    devices.Initialize();

    // Setup cpu
    std::cout << "Setup CPU..." << std::endl;
    CPU_8080 cpu;
    cpu.Initialize();
    cpu.LoadRom("invaders");
    cpu.Connect_io_dev(&devices);

    // Setup System Platform
    Platform_SFML platform;
    platform.Initialize(&devices, &cpu);


    platform.BeginEmulation();

    return 0;
}

#endif
