#ifndef PLTFRM_SFML
#define PLTFRM_SFML


#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>

class Io_devices;
class CPU_8080;

class Platform_SFML {
public:
    /* Public interface */
    void Initialize(Io_devices* devices, CPU_8080* cpu);
    void BeginEmulation();
    void PlaySound(uint8_t id);
    void StopSound(uint8_t id);

    /* Internal functions */
    sf::Color calculateOverlay(uint8_t hor, uint8_t ver);
    void CaptureInputs(sf::RenderWindow& window, Io_devices& devices);
    void DrawGraphics(CPU_8080& cpu, sf::RenderWindow& window);
    void SetupAudio();
    void UpdateSounds();

    /* All the gore below */
public:
    // Connectors
    Io_devices* m_devices;
    CPU_8080*   m_cpu;
    // Graphics
    sf::RenderWindow* window;
    // Sound
    sf::SoundBuffer shootBuffer;         sf::Sound shootSound;
    sf::SoundBuffer explosionBuffer;     sf::Sound explosionSound;
    sf::SoundBuffer fastinvader1Buffer;  sf::Sound fastinvader1Sound;
    sf::SoundBuffer fastinvader2Buffer;  sf::Sound fastinvader2Sound;
    sf::SoundBuffer fastinvader3Buffer;  sf::Sound fastinvader3Sound;
    sf::SoundBuffer fastinvader4Buffer;  sf::Sound fastinvader4Sound;
    sf::SoundBuffer invaderkilledBuffer; sf::Sound invaderkilledSound;
    sf::SoundBuffer uforepeatBuffer;     sf::Sound uforepeatSound;
    sf::SoundBuffer ufohitBuffer;        sf::Sound ufohitSound;

};

#endif
