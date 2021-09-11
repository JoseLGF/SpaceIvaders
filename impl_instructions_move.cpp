/*
 * Implementations for the CPU instructions in the move, load, etc group.
 *
 */

#include "i8080.h"

// Move immediate register B(000)
void CPU_8080::MVI_B(uint8_t data)
{
    b = data;
    pc += 2;
}

// Load immediate register pair D and E
void CPU_8080::LXI_D(uint8_t byte_d, uint8_t byte_e)
{
    d = byte_d;
    e = byte_e;
    pc += 3;
}

// Load immediate register pair H and L
void CPU_8080::LXI_H(uint8_t byte_h, uint8_t byte_l)
{
    h = byte_h;
    l = byte_l;
    pc += 3;
}

// Load A indirect from the address pointed to by the register pair DE
void CPU_8080::LDAX_D()
{
    uint16_t address = (d << 8) | e;
    a = memory[address];
    pc += 1;
}

// The contents of A are loaded to the address pointed to by the pair HL
void CPU_8080::MOV_M_A()
{
    uint16_t address = (h << 8) | l;
    memory[address] = a;
    pc += 1;
}
