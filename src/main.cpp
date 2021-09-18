/*
 * Main file for the spaceInvaders emulator program.
 */

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <iostream>
#include <cstdio>
#include "i8080.h"
#include "io_devices.h"

void patch_cpudiag_rom(CPU_8080& cpu)
{
    //Fix the first instruction to be JMP 0x100
    cpu.memory[0]=0xc3;
    cpu.memory[1]=0x00;
    cpu.memory[2]=0x01;

    //Fix the stack pointer from 0x6ad to 0x7ad
    // this 0x06 byte 112 in the code, which is
    // byte 112 + 0x100 = 368 in memory
    cpu.memory[368] = 0x7;

    //Skip DAA test
    cpu.memory[0x59c] = 0xc3; //JMP
    cpu.memory[0x59d] = 0xc2;
    cpu.memory[0x59e] = 0x05;
}


int main(int argc, char** argv)
{

    std::cout << "Space Invaders Emulator!" << std::endl;

    // Accept the first argument as hexadecimal number to fast-forward
    // to that address of the prram counter
    uint16_t ff_addr = strtol(argv[1], NULL, 16);
    std::cout   << "Fast forward to address "
                << std::hex << ff_addr <<std::endl;


    // Setup io devices
    Io_devices devices;
    devices.Initialize();

    // Setup cpu
    std::cout << "Setup CPU..." << std::endl;
    CPU_8080 cpu;
    cpu.Initialize();
    cpu.LoadRom("cpudiag.bin");
    patch_cpudiag_rom(cpu);
    cpu.Connect_io_dev(&devices);

    // Emulation loop
    std::cout << "Starting emulation..." << std::endl;
    while(cpu.Running() && (cpu.pc != ff_addr))
    {
        cpu.RegularInstruction();
    }
    while(cpu.Running())
    {
        cpu.DisassembleOp();
        char c = std::getchar();
        cpu.RegularInstruction();
        std::cout << "-------------------------------------" << std::endl;
        cpu.PrintState();
    }

    cpu.DumpMemory();
    std::cout << "Bye!" << std::endl;

    return 0;
}
