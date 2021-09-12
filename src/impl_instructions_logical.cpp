/*
 * Implementations for the CPU instructions in the logical group.
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
}
