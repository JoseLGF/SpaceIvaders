/*
 * File: io_devices.h
 * Description: Class for handling io devices for space invaders
 */

#ifndef IO_DEVICES
#define IO_DEVICES

#include <string>

#ifdef LIB_SFML
#include <SFML/Audio.hpp>
#endif
#ifdef LIB_SDL
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#endif

#include "platform.h"

class Io_devices {
public:
    /* Public interface */
	void    Initialize();
    uint8_t Read_device(uint8_t device_number);
    void    Write_device(uint8_t device_number, uint8_t data);
    void    UpdateSounds();
    void    SetupSounds();
    void    Connect(PLATFORMTYPE* platform);

public:
    uint8_t port1;
    uint8_t portout3;
    uint8_t portout5;

    /* 16-bit Shift register on device 3 */
    uint16_t shift_register_value;
    uint8_t  shift_register_offset;

    uint8_t Get_shift_register_result();
    void    Set_shift_register_result_offset(uint8_t amount);
    void    Fill_shift_register(uint8_t data);

    /* Sound-related */
    bool lastShootSoundActive    ;
    bool lastExplosionSoundActive;
    bool lastInvaderkilledSound  ;
    bool lastFastinvader1Sound   ;
    bool lastFastinvader2Sound   ;
    bool lastFastinvader3Sound   ;
    bool lastFastinvader4Sound   ;
    bool lastUforepeatSound;
    bool lastUfohitSound;

    bool currentShootSoundActive    ;
    bool currentExplosionSoundActive;
    bool currentInvaderkilledSound  ;
    bool currentFastinvader1Sound;
    bool currentFastinvader2Sound;
    bool currentFastinvader3Sound;
    bool currentFastinvader4Sound;
    bool currentUforepeatSound;
    bool currentUfohitSound;

    /* SFML sound interface */
#ifdef LIB_SFML
    sf::SoundBuffer shootBuffer;
    sf::SoundBuffer explosionBuffer;
    sf::SoundBuffer fastinvader1Buffer;
    sf::SoundBuffer fastinvader2Buffer;
    sf::SoundBuffer fastinvader3Buffer;
    sf::SoundBuffer fastinvader4Buffer;
    sf::SoundBuffer invaderkilledBuffer;
    sf::SoundBuffer uforepeatBuffer;
    sf::SoundBuffer ufohitBuffer;

    sf::Sound shootSound;
    sf::Sound explosionSound;
    sf::Sound fastinvader1Sound;
    sf::Sound fastinvader2Sound;
    sf::Sound fastinvader3Sound;
    sf::Sound fastinvader4Sound;
    sf::Sound invaderkilledSound;
    sf::Sound uforepeatSound;
    sf::Sound ufohitSound;

    void SFML_AudioSetup();
    void SFML_UpdateSounds();
#endif

    /* SDL sound interface */
#ifdef LIB_SDL
    Mix_Chunk *shootBuffer;
    Mix_Chunk *explosionBuffer;
    Mix_Chunk *fastinvader1Buffer;
    Mix_Chunk *fastinvader2Buffer;
    Mix_Chunk *fastinvader3Buffer;
    Mix_Chunk *fastinvader4Buffer;
    Mix_Chunk *invaderkilledBuffer;
    Mix_Chunk *uforepeatBuffer;
    Mix_Chunk *ufohitBuffer;

    void SDL_AudioSetup();
    void SDL_UpdateSounds();
#endif

};

#endif
