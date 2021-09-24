/*
 * File: io_devices.h
 * Description: Class for handling io devices for space invaders
 */

#ifndef IO_DEVICES
#define IO_DEVICES
#include <string>
#include <SFML/Audio.hpp>

class Io_devices {
public:
    /* Public interface */
	void    Initialize();
    uint8_t Read_device(uint8_t device_number);
    void    Write_device(uint8_t device_number, uint8_t data);
    void    UpdateSounds();
    void    SetupSounds();

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
    bool lastFastInvader1Sound   ;
    bool lastFastInvader2Sound   ;
    bool lastFastInvader3Sound   ;
    bool lastFastInvader4Sound   ;

    bool currentShootSoundActive    ;
    bool currentExplosionSoundActive;
    bool currentInvaderkilledSound  ;
    /* bool currentFastInvader1Sound; */
    /* bool currentFastInvader2Sound; */
    /* bool currentFastInvader3Sound; */
    /* bool currentFastInvader4Sound; */

    /* SFML sound interface */
    sf::SoundBuffer shootBuffer;
    sf::SoundBuffer explosionBuffer;
    sf::SoundBuffer fastinvader1Buffer;
    sf::SoundBuffer fastinvader2Buffer;
    sf::SoundBuffer fastinvader3Buffer;
    sf::SoundBuffer fastinvader4Buffer;
    sf::SoundBuffer invaderkilledBuffer;

    sf::Sound shootSound;
    sf::Sound explosionSound;
    sf::Sound fastinvader1Sound;
    sf::Sound fastinvader2Sound;
    sf::Sound fastinvader3Sound;
    sf::Sound fastinvader4Sound;
    sf::Sound invaderkilledSound;

};

#endif
