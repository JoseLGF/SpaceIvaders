/*
 * Implementations for the CPU instructions in the logical and rotate
 * groups.
 *
 */

#include "i8080.h"

#include <iostream>

void CPU_8080::ANA_r(uint8_t& r)
{
    uint8_t result = a & r;

    logical_flags(result);

    a = result;
    pc += 1;
    cycles += 4;
}

void CPU_8080::XRA_r(uint8_t& r)
{
    uint8_t result = a ^ r;



    logical_flags(result);

    a = result;
    pc += 1;
    cycles += 4;
}

void CPU_8080::ORA_r(uint8_t& r)
{
    uint8_t result = a | r;

    logical_flags(result);

    a = result;
    pc += 1;
    cycles += 4;
}

// ORA M, Or memory with A
void CPU_8080::ORA_M()
{
    uint16_t address = (h << 8) | l;
    uint8_t hl_content = MemoryRead(address);
    uint8_t result = a | hl_content;

    logical_flags(result);

    a = result;
    pc += 1;
    cycles += 7;
}

// Compare immediate with accumulator
void CPU_8080::CPI(uint8_t data)
{
    uint8_t result = a - data;

    cc.z  = (result == 0);
    cc.s  = ((result & 0x80) != 0);
    cc.p  = Parity(result);
    cc.cy = data > a;

    a = result;
    pc += 2;
    cycles += 7;
}

// Rotate A Right
void CPU_8080::RRC()
{
    uint8_t lsb = a & 0x01;
    a = (a >> 1) | (lsb << 7);

    cc.cy = lsb;

    pc += 1;
    cycles += 4;
}

// Rotate A Right through carry
void CPU_8080::RAR()
{
    uint8_t prev_cy = cc.cy;
    uint8_t lsb = a & 0x01;
    a = (a >> 1) | (prev_cy << 7);

    cc.cy = lsb;

    pc += 1;
    cycles += 4;
}

// Rotate A Left
void CPU_8080::RLC()
{
    uint8_t msb = a & 0x80;
    a = (a << 1) | (msb >> 7);

    cc.cy = msb != 0;

    pc += 1;
    cycles += 4;
}

// And immediate with A
void CPU_8080::ANI(uint8_t data)
{
    uint8_t result = a & data;

    logical_flags(result);

    a = result;
    pc += 2;
    cycles += 7;
}

// ORI, Or immediate with A
void CPU_8080::ORI(uint8_t data)
{
    uint8_t result = a | data;

    logical_flags(result);

    a = result;
    pc += 2;
    cycles += 7;
}
