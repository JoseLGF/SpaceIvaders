/*
 * Implementations for the CPU instructions in the logical and rotate
 * groups.
 *
 */

#include "i8080.h"

#include <iostream>

// XRA A, Exclusive or register A with A
void CPU_8080::XRA_A()
{
    uint8_t result = a ^ a;

    cc.z  = (result == 0);
    cc.s  = ((result & 0x80) != 0);
    cc.p  = Parity(result);
    cc.cy = 0; // Reset according to programmer's manual.

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

    cc.z  = (result == 0);
    cc.s  = ((result & 0x80) != 0);
    cc.p  = Parity(result);
    cc.cy = 0; // Reset according to programmer's manual.

    a = result;
    pc += 1;
    cycles += 7;
}

// ORA B, Or accumulator with B
void CPU_8080::ORA_B()
{
    uint8_t result = a | b;

    cc.z  = (result == 0);
    cc.s  = ((result & 0x80) != 0);
    cc.p  = Parity(result);
    cc.cy = 0; // Reset according to programmer's manual.

    a = result;
    pc += 1;
    cycles += 4;
}

// ORA H, Or memory with H
void CPU_8080::ORA_H()
{
    uint8_t result = a | h;

    cc.z  = (result == 0);
    cc.s  = ((result & 0x80) != 0);
    cc.p  = Parity(result);
    cc.cy = 0; // Reset according to programmer's manual.

    a = result;
    pc += 1;
    cycles += 4;
}

// ANA A, And register A with A
void CPU_8080::ANA_A()
{
    uint8_t result = a & a;

    cc.z  = (result == 0);
    cc.s  = ((result & 0x80) != 0);
    cc.p  = Parity(result);
    cc.cy = 0;

    a = result;
    pc += 1;
    cycles += 4;
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

    cc.cy = 0;
    cc.z  = (result == 0);
    cc.s  = ((result & 0x80) != 0);
    cc.p  = Parity(result);

    a = result;
    pc += 2;
    cycles += 7;
}

// ORI, Or immediate with A
void CPU_8080::ORI(uint8_t data)
{
    uint8_t result = a | data;

    cc.z  = (result == 0);
    cc.s  = ((result & 0x80) != 0);
    cc.p  = Parity(result);
    cc.cy = 0; // Reset according to programmer's manual.

    a = result;
    pc += 2;
    cycles += 7;
}
