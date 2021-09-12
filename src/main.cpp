/*
 * Main file for the spaceInvaders emulator program.
 */

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <iostream>
#include <cstdio>
#include "i8080.h"


void captureInputs(sf::RenderWindow& window){

    using sf::Keyboard;

    // check all the window's events that were triggered since the last iteration of the loop
    sf::Event event;
    while (window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed){
            window.close();
        }

        // Left and Right arrows to decrease or increase emulation speed
        /* if (event.type == sf::Event::KeyPressed){ */
        /*     if (Keyboard::isKeyPressed(Keyboard::Right)){ */
        /*         emulationSpeed += 50; */
        /*         std::cout << "Set emulation speed to " << emulationSpeed << std::endl; */
        /*         window.setFramerateLimit(emulationSpeed); */
        /*     } */
        /*     if (Keyboard::isKeyPressed(Keyboard::Left)){ */
        /*         emulationSpeed -= 50; */
        /*         std::cout << "Set emulation speed to " << emulationSpeed << std::endl; */
        /*         window.setFramerateLimit(emulationSpeed); */
        /*     } */
        /* } */

        /* // Enter to reset the game */
        /* if (event.type == sf::Event::KeyPressed){ */
        /*     if (Keyboard::isKeyPressed(Keyboard::Enter)){ */
        /*         myChip8.resetGame(); */
        /*     } */
        /* } */
    }
}

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

    // System Time
    sf::Clock System_time_clock;
    sf::Time System_elapsed_time = sf::Time::Zero;
    System_time_clock.restart();

    // Emulation loop
    std::cout << "Starting emulation..." << std::endl;
    while(window.isOpen() && cpu.Running())
    {
        System_elapsed_time += System_time_clock.restart();
        if (System_elapsed_time.asMilliseconds() > 17/*TimePerFrame*/){
            System_elapsed_time = sf::Time::Zero;

            cpu.EmulateCycles(33333);
            // Generate Half screen interrupt (1)
            cpu.Interrupt(1);
            cpu.EmulateCycles(33333);
            // Generate Full screen interrupt (2)
            cpu.Interrupt(2);
            // Draw Screen?
            // TODO
        }
        //cpu.PrintState();
        //cpu.ExecuteInstruction();
        captureInputs(window);
    }

    cpu.DumpMemory();
    std::cout << "Bye!" << std::endl;

    return 0;
}
