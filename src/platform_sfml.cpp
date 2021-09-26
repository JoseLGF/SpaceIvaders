/*
 * SFML platform module. Provides all SFML related functionality:
 * - Running the game in a loop
 * - Drawing graphics
 * - Capturing inputs
 * - Configuring and playing audio
 */
#ifdef LIB_SFML

#include "platform_sfml.h"

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <iostream>
#include "io_devices.h"

void Platform_SFML::Initialize(Io_devices* devices, CPU_8080* cpu)
{
    m_devices = devices;
    m_cpu = cpu;
    // Graphics
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    window = new sf::RenderWindow(
            sf::VideoMode(224, 256),
            "I8080 Emulator",
            sf::Style::Default, settings);
    window->setFramerateLimit(60);
}

void Platform_SFML::CaptureInputs(sf::RenderWindow& window, Io_devices& devices)
{
    using sf::Keyboard;

    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }

        if (event.type == sf::Event::KeyPressed) {
            switch (event.key.code) {
                case sf::Keyboard::C:
                    devices.port1 |= 0x01;
                    break;
                case sf::Keyboard::Enter:
                    devices.port1 |= 0x04;
                    break;
                case sf::Keyboard::Space:
                    devices.port1 |= 0x10;
                    break;
                case sf::Keyboard::Left:
                    devices.port1 |= 0x20;
                    break;
                case sf::Keyboard::Right:
                    devices.port1 |= 0x40;
                    break;
            }
        }

        if (event.type == sf::Event::KeyReleased) {
            switch (event.key.code) {
                case sf::Keyboard::C:
                    devices.port1 &= 0xfe;
                    break;
                case sf::Keyboard::Enter:
                    devices.port1 &= 0xfb;
                    break;
                case sf::Keyboard::Space:
                    devices.port1 &= 0xef;
                    break;
                case sf::Keyboard::Left:
                    devices.port1 &= 0xdf;
                    break;
                case sf::Keyboard::Right:
                    devices.port1 &= 0xbf;
                    break;
            }
        }
    }
}

sf::Color Platform_SFML::calculateOverlay(uint8_t hor, uint8_t ver)
{
    // Default Space invaders game overlay
    if (ver >= 256 - 32) {
        return sf::Color::White;
    }
    if (ver >= (256 - 32 - 32)) {
        return sf::Color::Red;
    }
    if (ver >= (256 - 32 - 32 - 120)) {
        return sf::Color::White;
    }
    if (ver >= (256 - 32 - 32 - 120 - 56)) {
        return sf::Color::Green;
    }
    // Last horizontal region divided in 3 parts
    if (hor <= 16) {
        return sf::Color::White;
    }
    if (hor <= (16 + 118)) {
        return sf::Color::Green;
    }
    return sf::Color::White;
}

void Platform_SFML::DrawGraphics(CPU_8080& cpu, sf::RenderWindow& window)
{
    // Create a 256x224 image filled with black color
    sf::Image image;
    image.create(224, 256, sf::Color::Black);

    // Draw the pixels from the memory locations 0x2400 - 0x3fff
    // into the window screen
    for(uint16_t v=0; v<224; v++){
        for(int16_t h=0; h<256; h++){
            uint16_t base_offset = 0x2400;
            uint16_t vertical_offset = 0x20 * v;
            uint16_t horizontal_offset = (h >> 3);
            uint16_t current_byte = base_offset
                                 + vertical_offset
                                 + horizontal_offset;
            uint8_t current_bit = (h % 8);


            bool thisPixel =
                (cpu.MemoryRead(current_byte) & (1 << current_bit)) != 0;
            sf::Color thisColor;

            // retrieve the current pixel color
            if(thisPixel){
                thisColor = calculateOverlay(v, h);
            }
            else{
                thisColor = sf::Color::Black;
            }

            // Rotate coordinates counter clockwise
            image.setPixel(v, 256 - h - 1, thisColor);
        }
    }

    // load image to texture
    sf::Texture texture;
    texture.loadFromImage(image);
    sf::Sprite sprite;
    sprite.setPosition(0, 0);
    sprite.setTexture(texture, false);

    // draw image in window
    window.draw(sprite);
}

void Platform_SFML::BeginEmulation()
{
    // System Time
    sf::Clock System_time_clock;
    sf::Time System_elapsed_time = sf::Time::Zero;
    System_time_clock.restart();

    // Emulation loop
    std::cout << "Starting emulation..." << std::endl;
    while(window->isOpen() && m_cpu->Running())
    {
    System_elapsed_time += System_time_clock.restart();
        if (System_elapsed_time.asMilliseconds() > 17/*TimePerFrame*/){
            System_elapsed_time = sf::Time::Zero;

            m_cpu->EmulateCycles(16666);
            // Generate Half screen interrupt (1)
            m_cpu->Interrupt(0xcf); // RST 1
            m_cpu->EmulateCycles(16666);
            // Generate Full screen interrupt (2)
            m_cpu->Interrupt(0xd7); // RST 2

            DrawGraphics(*m_cpu, *window);
            window->display();
        }
        CaptureInputs(*window, *m_devices);
        m_devices->UpdateSounds();

    }
    std::cout << "Bye!" << std::endl;
}

#endif
