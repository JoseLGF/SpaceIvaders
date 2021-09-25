/*
 * Main file for the spaceInvaders emulator program.
 * This main program uses the SDL library
 */
#ifdef LIB_SDL

#include <SDL2/SDL.h>

#include <iostream>
#include <cstdio>
#include "i8080.h"
#include "io_devices.h"

#ifdef TODO
void captureInputs(sf::RenderWindow& window, Io_devices& devices)
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
#endif

SDL_Color calculateOverlay(uint8_t hor, uint8_t ver)
{
#define WHITE {255,255,255}
#define RED   {255,  0,  0}
#define GREEN {  0,255,  0}
    // Default Space invaders game overlay
    if (ver >= 256 - 32) {
        return WHITE;
    }
    if (ver >= (256 - 32 - 32)) {
        return RED;
    }
    if (ver >= (256 - 32 - 32 - 120)) {
        return WHITE;
    }
    if (ver >= (256 - 32 - 32 - 120 - 56)) {
        return GREEN;
    }
    // Last horizontal region divided in 3 parts
    if (hor <= 16) {
        return WHITE;
    }
    if (hor <= (16 + 118)) {
        return GREEN;
    }
    return WHITE;
}

void drawGraphics(CPU_8080& cpu, SDL_Window * window, SDL_Renderer * renderer)
{
    SDL_Color pixelColor;
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);

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

            // retrieve the current pixel color
            if(thisPixel){
                pixelColor = calculateOverlay(v, h);
                SDL_SetRenderDrawColor(renderer, pixelColor.r, pixelColor.g, pixelColor.b, 255);
            }
            else{
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
            }

            // Rotate coordinates counter clockwise
            SDL_RenderDrawPoint(renderer, v, 256-h-1);
        }
    }
    SDL_RenderPresent(renderer);
}


int main(int argc, char** argv)
{

    std::cout << "Space Invaders Emulator!" << std::endl;

    // Setup SDL System
    SDL_Event event;
    SDL_Renderer *renderer;
    SDL_Window *window;
    int i;

    SDL_Init(SDL_INIT_VIDEO);
    SDL_CreateWindowAndRenderer(224, 256, 0, &window, &renderer);

    // Setup io devices
    Io_devices devices;
    devices.Initialize();

    // Setup cpu
    std::cout << "Setup CPU..." << std::endl;
    CPU_8080 cpu;
    cpu.Initialize();
    cpu.LoadRom("invaders");
    cpu.Connect_io_dev(&devices);

    // System Time
    uint32_t lastFrameTime = SDL_GetTicks();
    uint32_t currentTime = SDL_GetTicks();

    // Emulation loop
    std::cout << "Starting emulation..." << std::endl;
    while(cpu.Running()) { //TODO add window running check
        if (SDL_PollEvent(&event) && event.type == SDL_QUIT) {
           break;
        }
        currentTime = SDL_GetTicks();
        if ( (currentTime - lastFrameTime) > 17) {
            lastFrameTime = currentTime;

            cpu.EmulateCycles(16666);
            // Generate Half screen interrupt (1)
            cpu.Interrupt(0xcf); // RST 1
            cpu.EmulateCycles(16666);
            // Generate Full screen interrupt (2)
            cpu.Interrupt(0xd7); // RST 2

            drawGraphics(cpu, window, renderer);
        }
        /* captureInputs(window, devices); */
#ifdef SOUND_ENABLED
        devices.UpdateSounds();
#endif
    }
    std::cout << "Bye!" << std::endl;

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

#endif
