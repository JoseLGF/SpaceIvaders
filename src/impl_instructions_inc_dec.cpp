/*
 * Implementations for the CPU instructions in the increment and
 * decrement group.
 *
 */

#include "i8080.h"

#include <iostream>

// Increment HL register pair
void CPU_8080::INX_H()
{
    uint16_t num = (h << 8) | l;
    num++;
    h = (uint8_t) ((num >> 8) & 0xff);
    l = (uint8_t) (num & 0xff);
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

// Increment DE register pair
void CPU_8080::INX_D()
{
    uint16_t num = (d << 8) | e;
    num++;
    d = (uint8_t) ((num >> 8) & 0xff);
    e = (uint8_t) (num & 0xff);
    pc += 1;
    cycles += 5;
}

// Increment BC register pair
void CPU_8080::INX_B()
{
    uint16_t num = (b << 8) | c;
    num++;
    b = (uint8_t) ((num >> 8) & 0xff);
    c = (uint8_t) (num & 0xff);
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

// Decrement B register
void CPU_8080::DCR_B()
{
    uint8_t result = b - 1;

    cc.z = (result == 0);
    cc.s = ((result & 0x80) != 0);
    cc.p = Parity(result);
    // cc.c = Unaffected for this instruction

    b = result;
    pc += 1;
    cycles += 5;
}

// Increment B register
void CPU_8080::INR_B()
{
    uint8_t result = b + 1;

    cc.z = (result == 0);
    cc.s = ((result & 0x80) != 0);
    cc.p = Parity(result);
    // cc.c = Unaffected for this instruction

    b = result;
    pc += 1;
    cycles += 5;
}

// Increment A register
void CPU_8080::INR_A()
{
    uint8_t result = a + 1;

    cc.z = (result == 0);
    cc.s = ((result & 0x80) != 0);
    cc.p = Parity(result);
    // cc.c = Unaffected for this instruction

    a = result;
    pc += 1;
    cycles += 5;
}

// Decrement C register
void CPU_8080::DCR_C()
{
    uint8_t result = c - 1;

    cc.z = (result == 0);
    cc.s = ((result & 0x80) != 0);
    cc.p = Parity(result);
    // cc.c = Unaffected for this instruction

    c = result;
    pc += 1;
    cycles += 5;
}

// Decrement A register
void CPU_8080::DCR_A()
{
    uint8_t result = a - 1;

    cc.z = (result == 0);
    cc.s = ((result & 0x80) != 0);
    cc.p = Parity(result);
    // cc.c = Unaffected for this instruction

    a = result;
    pc += 1;
    cycles += 5;
}
