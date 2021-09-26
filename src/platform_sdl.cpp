/*
 * SDL platform module. Provides all SDL related functionality:
 * - Running the game in a loop
 * - Drawing graphics
 * - Capturing inputs
 * - Configuring and playing audio
 */
#ifdef LIB_SDL

#include "platform_sdl.h"

#include <SDL2/SDL.h>
#include <iostream>
#include "io_devices.h"

void Platform_SDL::Initialize(Io_devices* devices, CPU_8080* cpu)
{
    m_devices = devices;
    m_cpu = cpu;
    // Graphics
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    SDL_CreateWindowAndRenderer(224, 256, 0, &window, &renderer);
}

void Platform_SDL::CaptureInputs(Io_devices& devices)
{
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            exit(0);
        }
        if (event.type == SDL_KEYDOWN) {
            switch (event.key.keysym.sym) {
                case SDLK_c:        devices.port1 |= 0x01; break;
                case SDLK_RETURN:   devices.port1 |= 0x04; break;
                case SDLK_SPACE:    devices.port1 |= 0x10; break;
                case SDLK_LEFT:     devices.port1 |= 0x20; break;
                case SDLK_RIGHT:    devices.port1 |= 0x40; break;
            }
        }

        if (event.type == SDL_KEYUP) {
            switch (event.key.keysym.sym) {
                case SDLK_c:        devices.port1 &= 0xfe; break;
                case SDLK_RETURN:   devices.port1 &= 0xfb; break;
                case SDLK_SPACE:    devices.port1 &= 0xef; break;
                case SDLK_LEFT:     devices.port1 &= 0xdf; break;
                case SDLK_RIGHT:    devices.port1 &= 0xbf; break;
            }
        }
    }
}

SDL_Color Platform_SDL::calculateOverlay(uint8_t hor, uint8_t ver)
{
#define WHITE {255,255,255}
#define RED   {255,  0,  0}
#define GREEN {  0,255,  0}
    // Default Space invaders game overlay
    if (ver >= 256 - 32) { return WHITE; }
    if (ver >= (256 - 32 - 32)) { return RED; }
    if (ver >= (256 - 32 - 32 - 120)) { return WHITE; }
    if (ver >= (256 - 32 - 32 - 120 - 56)) { return GREEN; }
    // Last horizontal region divided in 3 parts
    if (hor <= 16) { return WHITE; }
    if (hor <= (16 + 118)) { return GREEN; }
    return WHITE;
}

void Platform_SDL::DrawGraphics(CPU_8080& cpu)
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

        bool thisPixel = (cpu.MemoryRead(current_byte) & (1 << current_bit)) != 0;

        // retrieve the current pixel color
        if(thisPixel){
            pixelColor = calculateOverlay(v, h);
            SDL_SetRenderDrawColor(renderer, pixelColor.r, pixelColor.g, pixelColor.b, 255);
        }
        else {
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
        }

        // Rotate coordinates counter clockwise
        SDL_RenderDrawPoint(renderer, v, 256-h-1);
    }
    }
    SDL_RenderPresent(renderer);
}

void Platform_SDL::BeginEmulation()
{
    // System Time
    uint32_t lastFrameTime = SDL_GetTicks();
    uint32_t currentTime = SDL_GetTicks();

    // Emulation loop
    std::cout << "Starting emulation..." << std::endl;
    while(m_cpu->Running()) {
        currentTime = SDL_GetTicks();
        if ( (currentTime - lastFrameTime) > 17) {
            lastFrameTime = currentTime;

            m_cpu->EmulateCycles(16666);
            // Generate Half screen interrupt (1)
            m_cpu->Interrupt(0xcf); // RST 1
            m_cpu->EmulateCycles(16666);
            // Generate Full screen interrupt (2)
            m_cpu->Interrupt(0xd7); // RST 2

            DrawGraphics(*m_cpu);
        }
        CaptureInputs(*m_devices);
#ifdef LIB_SDL
        m_devices->UpdateSounds();
#endif
    }
    std::cout << "Bye!" << std::endl;
}

#endif
