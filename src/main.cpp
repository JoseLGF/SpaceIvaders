/*
 * Main file for the spaceInvaders emulator program.
 */

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <iostream>
#include <cstdio>
#include "i8080.h"

int main(int argc, char** argv) {

    std::cout << "Space Invaders Emulator!" << std::endl;

    // Setup sfml Graphics
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    sf::RenderWindow window(
            sf::VideoMode(256, 224),
            "I8080 Emulator",
            sf::Style::Default, settings);
    window.setFramerateLimit(60);

    // Setup cpu
    std::cout << "Setup CPU..." << std::endl;
    CPU_8080 cpu;
    cpu.Initialize();
    cpu.LoadRom();

    // Emulation loop
    std::cout << "Starting emulation..." << std::endl;
    while(window.isOpen() && cpu.Running())
    {
        cpu.PrintState();
        cpu.ExecuteInstruction();
    }

    cpu.DumpMemory();
    std::cout << "Bye!" << std::endl;

    return 0;
}
