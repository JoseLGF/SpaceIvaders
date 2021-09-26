/*
 * File: memory.h
 * Description: Abstracts the memory connected to the 8080 cpu.
 * */

#include <string>

class Memory {
public:
    void    Initialize();
    void    LoadRom(std::string filename);
    uint8_t Read(uint16_t address);
    void    Write(uint16_t address, uint8_t data);

private:
    uint8_t memory[0x4000];
};
