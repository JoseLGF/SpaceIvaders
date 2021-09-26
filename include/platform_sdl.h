#ifndef PLATF_SDL
#define PLATF_SDL

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

class Io_devices;
class CPU_8080;

class Platform_SDL {
public:
    /* Public interface */
    void Initialize(Io_devices* devices, CPU_8080* cpu);
    void BeginEmulation();
    void PlaySound(uint8_t id);
    void StopSound(uint8_t id);

    /* Internal functions */
    SDL_Color calculateOverlay(uint8_t hor, uint8_t ver);
    void CaptureInputs(Io_devices& devices);
    void DrawGraphics(CPU_8080& cpu);
    void SetupAudio();
    void UpdateSounds();

    /* All gore below */
public:
    // Connectors
    Io_devices* m_devices;
    CPU_8080*   m_cpu;
    // Graphics
    SDL_Renderer * renderer;
    SDL_Window   * window;
    // Sound
#define CHNL_SHOOT  0
#define CHNL_EXPLSN 1
#define CHNL_INVKLD 2
#define CHNL_FSINV1 3
#define CHNL_FSINV2 4
#define CHNL_FSINV3 5
#define CHNL_FSINV4 6
#define CHNL_UFORPT 7
#define CHNL_UFOHIT 8

    Mix_Chunk *shootBuffer;
    Mix_Chunk *explosionBuffer;
    Mix_Chunk *fastinvader1Buffer;
    Mix_Chunk *fastinvader2Buffer;
    Mix_Chunk *fastinvader3Buffer;
    Mix_Chunk *fastinvader4Buffer;
    Mix_Chunk *invaderkilledBuffer;
    Mix_Chunk *uforepeatBuffer;
    Mix_Chunk *ufohitBuffer;
};

#endif
