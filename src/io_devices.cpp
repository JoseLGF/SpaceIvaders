#include "io_devices.h"
#include <iostream>
#include <fstream>


void Io_devices::Initialize()
{
    port1 = 0x00;
    portout3 = 0x00;
    portout5 = 0x00;

    shift_register_value = 0x0000;

    lastShootSoundActive     = false;
    lastExplosionSoundActive = false;
    lastInvaderkilledSound   = false;
    lastFastinvader1Sound    = false;
    lastFastinvader2Sound    = false;
    lastFastinvader3Sound    = false;
    lastFastinvader4Sound    = false;
    lastUforepeatSound       = false;

#ifdef LIB_SFML
    SFML_AudioSetup();
#endif
#ifdef LIB_SDL
    SDL_AudioSetup();
#endif
}

void Io_devices::Connect(PLATFORMTYPE* platform)
{
    m_platform = platform;
}

uint8_t Io_devices::Read_device(uint8_t device_number)
{
    switch(device_number)
    {
        case 0x1: return port1; break;
        case 0x3: return Get_shift_register_result(); break;
        default : return 0u;
    }
}

void Io_devices::Write_device(uint8_t device_number, uint8_t data)
{
    switch(device_number)
    {
        case 0x2: Set_shift_register_result_offset(data);   break;
        case 0x3: portout3 = data;                          break;
        case 0x5: portout5 = data;                          break;
        case 0x4: Fill_shift_register(data);                break;
        default : break;
    }
}

uint8_t Io_devices::Get_shift_register_result()
{
    return
        ((shift_register_value >> (8-shift_register_offset)) & 0xff);
}

void Io_devices::Set_shift_register_result_offset(uint8_t amount)
{
    shift_register_offset = amount & 0x7;
}

void Io_devices::Fill_shift_register(uint8_t data)
{
    shift_register_value =
        (shift_register_value >> 8) |
        (data << 8);
}

void Io_devices::UpdateSounds()
{
    currentUforepeatSound       = ((portout3 & 0x01) != 0);
    currentShootSoundActive     = ((portout3 & 0x02) != 0);
    currentExplosionSoundActive = ((portout3 & 0x04) != 0);
    currentInvaderkilledSound   = ((portout3 & 0x08) != 0);
    currentFastinvader1Sound    = ((portout5 & 0x01) != 0);
    currentFastinvader2Sound    = ((portout5 & 0x02) != 0);
    currentFastinvader3Sound    = ((portout5 & 0x04) != 0);
    currentFastinvader4Sound    = ((portout5 & 0x08) != 0);
    currentUfohitSound          = ((portout5 & 0x10) != 0);
    if (!lastShootSoundActive && currentShootSoundActive) { m_platform->PlaySound(SNDID_SHOOT); }
    if (lastShootSoundActive && !currentShootSoundActive) { m_platform->StopSound(SNDID_SHOOT); }
    lastShootSoundActive = currentShootSoundActive;

    if (!lastExplosionSoundActive && currentExplosionSoundActive) { m_platform->PlaySound(SNDID_EXPLOSION); }
    if (lastExplosionSoundActive && !currentExplosionSoundActive) { m_platform->StopSound(SNDID_EXPLOSION); }
    lastExplosionSoundActive = currentExplosionSoundActive;

    if (!lastInvaderkilledSound && currentInvaderkilledSound) { m_platform->PlaySound(SNDID_INVKLLD); }
    if (lastInvaderkilledSound && !currentInvaderkilledSound) { m_platform->StopSound(SNDID_INVKLLD); }
    lastInvaderkilledSound = currentInvaderkilledSound;

    if (!lastFastinvader1Sound && currentFastinvader1Sound) { m_platform->PlaySound(SNDID_FSTINV1); }
    if (lastFastinvader1Sound && !currentFastinvader1Sound) { m_platform->StopSound(SNDID_FSTINV1); }
    lastFastinvader1Sound = currentFastinvader1Sound;

    if (!lastFastinvader2Sound && currentFastinvader2Sound) { m_platform->PlaySound(SNDID_FSTINV2); }
    if (lastFastinvader2Sound && !currentFastinvader2Sound) { m_platform->StopSound(SNDID_FSTINV2); }
    lastFastinvader2Sound = currentFastinvader2Sound;

    if (!lastFastinvader3Sound && currentFastinvader3Sound) { m_platform->PlaySound(SNDID_FSTINV3); }
    if (lastFastinvader3Sound && !currentFastinvader3Sound) { m_platform->StopSound(SNDID_FSTINV3); }
    lastFastinvader3Sound = currentFastinvader3Sound;

    if (!lastFastinvader4Sound && currentFastinvader4Sound) { m_platform->PlaySound(SNDID_FSTINV4); }
    if (lastFastinvader4Sound && !currentFastinvader4Sound) { m_platform->StopSound(SNDID_FSTINV4); }
    lastFastinvader4Sound = currentFastinvader4Sound;

    if (!lastUforepeatSound && currentUforepeatSound) { m_platform->PlaySound(SNDID_UFOREPT); }
    if (lastUforepeatSound && !currentUforepeatSound) { m_platform->StopSound(SNDID_UFOREPT); }
    lastUforepeatSound = currentUforepeatSound;

    if (!lastUfohitSound && currentUfohitSound) { m_platform->PlaySound(SNDID_UFOHIT); }
    if (lastUfohitSound && !currentUfohitSound) { m_platform->StopSound(SNDID_UFOHIT); }
    lastUfohitSound = currentUfohitSound;
}

#ifdef LIB_SFML
void Io_devices::SFML_UpdateSounds()
{
}

void Io_devices::SFML_AudioSetup()
{
    // Load sounds from files
    if (
        !uforepeatBuffer.loadFromFile("./sounds/ufo_highpitch.wav")     ||
        !ufohitBuffer.loadFromFile("./sounds/ufo_lowpitch.wav")         ||
        !shootBuffer.loadFromFile("./sounds/shoot.wav")                 ||
        !explosionBuffer.loadFromFile("./sounds/explosion.wav")         ||
        !fastinvader1Buffer.loadFromFile("./sounds/fastinvader1.wav")   ||
        !fastinvader2Buffer.loadFromFile("./sounds/fastinvader2.wav")   ||
        !fastinvader3Buffer.loadFromFile("./sounds/fastinvader3.wav")   ||
        !fastinvader4Buffer.loadFromFile("./sounds/fastinvader4.wav")   ||
        !invaderkilledBuffer.loadFromFile("./sounds/invaderkilled.wav")
       )
    {
        std::cout << "Error loading sounds." << std::endl;
        return;
    }
    std::cout << "Sounds loaded correctly." << std::endl;


    // Continue setup
    uforepeatSound.setBuffer(uforepeatBuffer);
    ufohitSound.setBuffer(ufohitBuffer);
    shootSound.setBuffer(shootBuffer);
    explosionSound.setBuffer(explosionBuffer);
    fastinvader1Sound.setBuffer(fastinvader1Buffer);
    fastinvader2Sound.setBuffer(fastinvader2Buffer);
    fastinvader3Sound.setBuffer(fastinvader3Buffer);
    fastinvader4Sound.setBuffer(fastinvader4Buffer);
    invaderkilledSound.setBuffer(invaderkilledBuffer);
    uforepeatSound.setLoop(true);
    std::cout << "Sounds setup complete." << std::endl;
}
#endif

#ifdef LIB_SDL
void Io_devices::SDL_AudioSetup()
{
    //Initialize SDL_mixer
    if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 )
    {
        printf( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() );
    }

    // Load sounds from files
    uforepeatBuffer     = Mix_LoadWAV("./sounds/ufo_highpitch.wav");
    ufohitBuffer        = Mix_LoadWAV("./sounds/ufo_lowpitch.wav");
    shootBuffer         = Mix_LoadWAV("./sounds/shoot.wav");
    explosionBuffer     = Mix_LoadWAV("./sounds/explosion.wav");
    fastinvader1Buffer  = Mix_LoadWAV("./sounds/fastinvader1.wav");
    fastinvader2Buffer  = Mix_LoadWAV("./sounds/fastinvader2.wav");
    fastinvader3Buffer  = Mix_LoadWAV("./sounds/fastinvader3.wav");
    fastinvader4Buffer  = Mix_LoadWAV("./sounds/fastinvader4.wav");
    invaderkilledBuffer = Mix_LoadWAV("./sounds/invaderkilled.wav");

    if (
        (shootBuffer         == NULL) ||
        (uforepeatBuffer     == NULL) ||
        (ufohitBuffer        == NULL) ||
        (shootBuffer         == NULL) ||
        (explosionBuffer     == NULL) ||
        (fastinvader1Buffer  == NULL) ||
        (fastinvader2Buffer  == NULL) ||
        (fastinvader3Buffer  == NULL) ||
        (fastinvader4Buffer  == NULL) ||
        (invaderkilledBuffer == NULL)
    )
    {
        std::cout << "SDL: Error loading audio files." << std::endl;
    }
    else
    {
        // Continue setup
        /* uforepeatSound.setLoop(true); */
        std::cout << "Sounds setup complete." << std::endl;
    }
}

void Io_devices::SDL_UpdateSounds()
{
#define CHNL_SHOOT  0
#define CHNL_EXPLSN 1
#define CHNL_INVKLD 2
#define CHNL_FSINV1 3
#define CHNL_FSINV2 4
#define CHNL_FSINV3 5
#define CHNL_FSINV4 6
#define CHNL_UFORPT 7
#define CHNL_UFOHIT 8
    currentUforepeatSound       = ((portout3 & 0x01) != 0);
    currentShootSoundActive     = ((portout3 & 0x02) != 0);
    currentExplosionSoundActive = ((portout3 & 0x04) != 0);
    currentInvaderkilledSound   = ((portout3 & 0x08) != 0);
    currentFastinvader1Sound    = ((portout5 & 0x01) != 0);
    currentFastinvader2Sound    = ((portout5 & 0x02) != 0);
    currentFastinvader3Sound    = ((portout5 & 0x04) != 0);
    currentFastinvader4Sound    = ((portout5 & 0x08) != 0);
    currentUfohitSound          = ((portout5 & 0x10) != 0);

    if (!lastShootSoundActive && currentShootSoundActive) { Mix_PlayChannel(CHNL_SHOOT, shootBuffer, 0);}
    if (lastShootSoundActive && !currentShootSoundActive) { Mix_HaltChannel(CHNL_SHOOT); }
    lastShootSoundActive = currentShootSoundActive;

    if (!lastExplosionSoundActive && currentExplosionSoundActive) { Mix_PlayChannel(CHNL_EXPLSN, explosionBuffer, 0); }
    if (lastExplosionSoundActive && !currentExplosionSoundActive) { Mix_HaltChannel(CHNL_EXPLSN); }
    lastExplosionSoundActive = currentExplosionSoundActive;

    if (!lastInvaderkilledSound && currentInvaderkilledSound) { Mix_PlayChannel(CHNL_INVKLD, invaderkilledBuffer, 0); }
    if (lastInvaderkilledSound && !currentInvaderkilledSound) { Mix_HaltChannel(CHNL_INVKLD); }
    lastInvaderkilledSound = currentInvaderkilledSound;

    if (!lastFastinvader1Sound && currentFastinvader1Sound) { Mix_PlayChannel(CHNL_FSINV1, fastinvader1Buffer, 0); }
    if (lastFastinvader1Sound && !currentFastinvader1Sound) { Mix_HaltChannel(CHNL_FSINV1); }
    lastFastinvader1Sound = currentFastinvader1Sound;

    if (!lastFastinvader2Sound && currentFastinvader2Sound) { Mix_PlayChannel(CHNL_FSINV2, fastinvader2Buffer, 0); }
    if (lastFastinvader2Sound && !currentFastinvader2Sound) { Mix_HaltChannel(CHNL_FSINV2); }
    lastFastinvader2Sound = currentFastinvader2Sound;

    if (!lastFastinvader3Sound && currentFastinvader3Sound) { Mix_PlayChannel(CHNL_FSINV3, fastinvader3Buffer, 0); }
    if (lastFastinvader3Sound && !currentFastinvader3Sound) { Mix_HaltChannel(CHNL_FSINV3); }
    lastFastinvader3Sound = currentFastinvader3Sound;

    if (!lastFastinvader4Sound && currentFastinvader4Sound) { Mix_PlayChannel(CHNL_FSINV4, fastinvader4Buffer, 0); }
    if (lastFastinvader4Sound && !currentFastinvader4Sound) { Mix_HaltChannel(CHNL_FSINV4); }
    lastFastinvader4Sound = currentFastinvader4Sound;

    if (!lastUforepeatSound && currentUforepeatSound) { Mix_PlayChannel(CHNL_UFORPT, uforepeatBuffer, -1); }
    if (lastUforepeatSound && !currentUforepeatSound) { Mix_HaltChannel(CHNL_UFORPT); }
    lastUforepeatSound = currentUforepeatSound;

    if (!lastUfohitSound && currentUfohitSound) { Mix_PlayChannel(CHNL_UFOHIT, ufohitBuffer, 0); }
    if (lastUfohitSound && !currentUfohitSound) { Mix_HaltChannel(CHNL_UFOHIT); }
    lastUfohitSound = currentUfohitSound;
}
#endif
