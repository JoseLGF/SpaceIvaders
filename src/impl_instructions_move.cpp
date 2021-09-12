/*
 * Implementations for the CPU instructions in the move, load, etc group.
 *
 */

#include "i8080.h"

// Move immediate register B
void CPU_8080::MVI_B(uint8_t data)
{
    b = data;
    pc += 2;
    cycles += 7;
}

// Move immediate register A
void CPU_8080::MVI_A(uint8_t data)
{
    a = data;
    pc += 2;
    cycles += 7;
}

// Move immediate register C
void CPU_8080::MVI_C(uint8_t data)
{
    c = data;
    pc += 2;
    cycles += 7;
}

// Move immediate register H
void CPU_8080::MVI_H(uint8_t data)
{
    h = data;
    pc += 2;
    cycles += 7;
}

// Load immediate register pair D and E
void CPU_8080::LXI_D(uint8_t byte_d, uint8_t byte_e)
{
    d = byte_d;
    e = byte_e;
    pc += 3;
    cycles += 10;
}

// Load immediate register pair B and C
void CPU_8080::LXI_B(uint8_t byte_b, uint8_t byte_c)
{
    b = byte_b;
    c = byte_c;
    pc += 3;
    cycles += 10;
}

// Load immediate register pair H and L
void CPU_8080::LXI_H(uint8_t byte_h, uint8_t byte_l)
{
    h = byte_h;
    l = byte_l;
    pc += 3;
    cycles += 10;
}

// Load A indirect from the address pointed to by the register pair DE
void CPU_8080::LDAX_D()
{
    uint16_t address = (d << 8) | e;
    a = memory[address];
    pc += 1;
    cycles += 7;
}

// The contents of A are loaded to the address pointed to by the pair HL
void CPU_8080::MOV_M_A()
{
    uint16_t address = (h << 8) | l;
    memory[address] = a;
    pc += 1;
    cycles += 7;
}

// Move the value of H into A
void CPU_8080::MOV_A_H()
{
    a = h;
    pc += 1;
    cycles += 5;
}

// Move the value of A into L
void CPU_8080::MOV_L_A()
{
    l = a;
    pc += 1;
    cycles += 5;
}

// Store A direct
void CPU_8080::STA(uint8_t byte_h, uint8_t byte_l)
{
    uint16_t address = (byte_h << 8) | byte_l;
    memory[address] = a;
    pc += 3;
    cycles += 13;
}

// Load A direct
void CPU_8080::LDA(uint8_t byte_h, uint8_t byte_l)
{
    uint16_t address = (byte_h << 8) | byte_l;
    a = memory[address];
    pc += 3;
    cycles += 13;
}

// Move immediate Memory
void CPU_8080::MVI_M(uint8_t data)
{
    uint16_t address = (h << 8) | l;
    memory[address] = data;
    pc += 2;
    cycles += 10;
}

// Exchange the contents of register pairs HL and DE
void CPU_8080::XCHG()
{
    uint8_t tmp;
    tmp = h;
    h = d;
    d = tmp;
    tmp = l;
    l = e;
    e = tmp;

    pc += 1;
    cycles += 4;
}

// Contents of the address pointed to by the pair HL are loaded to E
void CPU_8080::MOV_E_M()
{
    uint16_t address = (h << 8) | l;
    e = memory[address];
    pc += 1;
    cycles += 7;
}

// Contents of the address pointed to by the pair HL are loaded to D
void CPU_8080::MOV_D_M()
{
    uint16_t address = (h << 8) | l;
    d = memory[address];
    pc += 1;
    cycles += 7;
}

// Contents of the address pointed to by the pair HL are loaded to A
void CPU_8080::MOV_A_M()
{
    uint16_t address = (h << 8) | l;
    a = memory[address];
    pc += 1;
    cycles += 7;
}

// Contents of the address pointed to by the pair HL are loaded to H
void CPU_8080::MOV_H_M()
{
    uint16_t address = (h << 8) | l;
    h = memory[address];
    pc += 1;
    cycles += 7;
}

// Move D to A
void CPU_8080::MOV_A_D()
{
    a = d;
    pc += 1;
    cycles += 5;
}

// Move E to A
void CPU_8080::MOV_A_E()
{
    a = e;
    pc += 1;
    cycles += 5;
}

// Store A indirect
void CPU_8080::STAX_B()
{
    uint16_t address = (b << 8) | c;
    memory[address] = a;
    pc += 1;
    cycles += 7;
}
