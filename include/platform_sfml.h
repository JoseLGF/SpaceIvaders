#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include "io_devices.h"
#include "i8080.h"

class Platform_SFML {
public:
    /* Public interface */
    void Initialize(Io_devices* devices, CPU_8080* cpu);
    void BeginEmulation();

    /* Internal functions */
    sf::Color calculateOverlay(uint8_t hor, uint8_t ver);
    void CaptureInputs(sf::RenderWindow& window, Io_devices& devices);
    void DrawGraphics(CPU_8080& cpu, sf::RenderWindow& window);

    /* All the gore below */
public:
    // Connectors
    Io_devices* m_devices;
    CPU_8080*   m_cpu;
    // Graphics
    sf::RenderWindow* window;
};
