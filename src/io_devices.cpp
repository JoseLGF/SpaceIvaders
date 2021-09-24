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

    currentShootSoundActive     = ((portout3 & 0x02) != 0);
    currentExplosionSoundActive = ((portout3 & 0x04) != 0);
    currentInvaderkilledSound   = ((portout3 & 0x08) != 0);

    SetupSounds();
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
    currentShootSoundActive     = ((portout3 & 0x02) != 0);
    currentExplosionSoundActive = ((portout3 & 0x04) != 0);
    currentInvaderkilledSound   = ((portout3 & 0x08) != 0);
    /* currentFastInvader1Sound    = ((devices.portout5 & 0x01) != 0); */
    /* currentFastInvader2Sound    = ((devices.portout5 & 0x02) != 0); */
    /* currentFastInvader3Sound    = ((devices.portout5 & 0x04) != 0); */
    /* currentFastInvader4Sound    = ((devices.portout5 & 0x08) != 0); */

    if (!lastShootSoundActive && currentShootSoundActive) { shootSound.play(); }
    if (lastShootSoundActive && !currentShootSoundActive) { shootSound.stop(); }
    lastShootSoundActive = currentShootSoundActive;

    if (!lastExplosionSoundActive && currentExplosionSoundActive) { explosionSound.play(); }
    if (lastExplosionSoundActive && !currentExplosionSoundActive) { explosionSound.stop(); }
    lastExplosionSoundActive = currentExplosionSoundActive;

    if (!lastInvaderkilledSound && currentInvaderkilledSound) { invaderkilledSound.play(); }
    if (lastInvaderkilledSound && !currentInvaderkilledSound) { invaderkilledSound.stop(); }
    lastInvaderkilledSound = currentInvaderkilledSound;
}

void Io_devices::SetupSounds()
{
    // Load sounds from files
    if (
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
    shootSound.setBuffer(shootBuffer);
    explosionSound.setBuffer(explosionBuffer);
    fastinvader1Sound.setBuffer(fastinvader1Buffer);
    fastinvader2Sound.setBuffer(fastinvader2Buffer);
    fastinvader3Sound.setBuffer(fastinvader3Buffer);
    fastinvader4Sound.setBuffer(fastinvader4Buffer);
    invaderkilledSound.setBuffer(invaderkilledBuffer);
    std::cout << "Sounds setup complete." << std::endl;
}
