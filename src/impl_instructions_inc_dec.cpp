/*
 * Implementations for the CPU instructions in the increment and
 * decrement group.
 *
 */

#include "i8080.h"

#include <iostream>

void CPU_8080::INR_r(uint8_t& r)
{
    uint8_t result = r + 1;

    cc.z = (result == 0);
    cc.s = ((result & 0x80) != 0);
    cc.p = Parity(result);
    // cc.c = Unaffected for this instruction

    r = result;
    pc += 1;
    cycles += 5;
}

void CPU_8080::DCR_r(uint8_t& r)
{
    uint8_t result = r - 1;

    cc.z = (result == 0);
    cc.s = ((result & 0x80) != 0);
    cc.p = Parity(result);
    // cc.c = Unaffected for this instruction

    r = result;
    pc += 1;
    cycles += 5;
}

// Increment register pair
void CPU_8080::INX(uint8_t& r1, uint8_t& r2)
{
    uint16_t num = (r1 << 8) | r2;
    num++;
    r1 = (uint8_t) ((num >> 8) & 0xff);
    r2 = (uint8_t) (num & 0xff);
    pc += 1;
    cycles += 5;
}

// Decrement HL register pair
void CPU_8080::DCX_H()
{
    uint16_t num = (h << 8) | l;
    num--;
    h = (uint8_t) ((num >> 8) & 0xff);
    l = (uint8_t) (num & 0xff);
    pc += 1;
    cycles += 5;
}


// Decrement memory
void CPU_8080::DCR_M()
{
    uint16_t address = (h << 8) | l;
    uint8_t num = MemoryRead(address);
    num--;
    MemoryWrite(address, num);

    cc.z = (num == 0);
    cc.s = ((num & 0x80) != 0);
    cc.p = Parity(num);
    // cc.c = Unaffected for this instruction

    pc += 1;
    cycles += 10;
}

