/*
 * Implementations for the CPU instructions in the move, load, etc group.
 *
 */

#include "i8080.h"

void CPU_8080::MVI(uint8_t& reg, uint8_t data)
{
    reg = data;
    pc += 2;
    cycles += 7;
}

void CPU_8080::MOV_r_r(uint8_t& r1, uint8_t& r2)
{
    r1 = r2;
    pc += 1;
    cycles += 5;
}

void CPU_8080::MOV_r_m(uint8_t& r)
{
    uint16_t address = (h << 8) | l;
    r = MemoryRead(address);
    pc += 1;
    cycles += 7;
}

void CPU_8080::MOV_m_r(uint8_t& r)
{
    uint16_t address = (h << 8) | l;
    MemoryWrite(address, r);
    pc += 1;
    cycles += 7;
}

// Load immediate register pair
void CPU_8080::LXI(uint8_t& r1, uint8_t& r2, uint8_t d1, uint8_t d2)
{
    r1 = d1;
    r2 = d2;
    pc += 3;
    cycles += 10;
}

// Load A indirect from the address pointed to by the register pair
void CPU_8080::LDAX(uint8_t& r1, uint8_t& r2)
{
    uint16_t address = (r1 << 8) | r2;
    a = MemoryRead(address);
    pc += 1;
    cycles += 7;
}

// Store A direct
void CPU_8080::STA(uint8_t byte_h, uint8_t byte_l)
{
    uint16_t address = (byte_h << 8) | byte_l;
    MemoryWrite(address, a);
    pc += 3;
    cycles += 13;
}

// Load A direct
void CPU_8080::LDA(uint8_t byte_h, uint8_t byte_l)
{
    uint16_t address = (byte_h << 8) | byte_l;
    a = MemoryRead(address);
    pc += 3;
    cycles += 13;
}

// Move immediate Memory
void CPU_8080::MVI_M(uint8_t data)
{
    uint16_t address = (h << 8) | l;
    MemoryWrite(address, data);
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


// Store A indirect
void CPU_8080::STAX_B()
{
    uint16_t address = (b << 8) | c;
    MemoryWrite(address, a);
    pc += 1;
    cycles += 7;
}

// Load HL pair direct
void CPU_8080::LHLD(uint8_t hi, uint8_t lo)
{
    uint16_t address = (hi << 8) | lo;
    l = MemoryRead(address);
    h = MemoryRead(address+1);
    pc += 3;
    cycles += 16;
}

// Store HL pair direct
void CPU_8080::SHLD(uint8_t hi, uint8_t lo)
{
    uint16_t address = (hi << 8) | lo;
    MemoryWrite(address, l);
    MemoryWrite(address+1, h);
    pc += 3;
    cycles += 16;
}
