#include <SDL2/SDL.h>
#include "io_devices.h"
#include "i8080.h"

class Platform_SDL {
public:
    /* Public interface */
    void Initialize(Io_devices* devices, CPU_8080* cpu);
    void BeginEmulation();

    /* Internal functions */
    SDL_Color calculateOverlay(uint8_t hor, uint8_t ver);
    void CaptureInputs(Io_devices& devices);
    void DrawGraphics(CPU_8080& cpu);

    /* All gore below */
public:
    // Connectors
    Io_devices* m_devices;
    CPU_8080*   m_cpu;
    // Graphics
    SDL_Renderer * renderer;
    SDL_Window   * window;

};
