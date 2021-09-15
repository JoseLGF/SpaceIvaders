/*
 * Implementations for the CPU instructions in the add and subtract
 * groups.
 *
 */

#include "i8080.h"

#include <iostream>
#include <string>

// Add r1&r2 to H&L
void CPU_8080::DAD(uint8_t& r1, uint8_t& r2)
{
    uint16_t r1_and_r2 = (r1 << 8) | r2;
    uint16_t h_and_l = (h << 8) | l;
    uint32_t result  = (uint32_t)r1_and_r2 + (uint32_t)h_and_l;
    h = ( (result >> 8) & 0xff );
    l = result & 0xff;

    cc.cy = (result > 0xffff);

    cycles += 10;
    pc += 1;
}

// Add memory to A
void CPU_8080::ADD_M()
{
    uint16_t address = (h << 8) | l;
    uint8_t hl_content = MemoryRead(address);
    uint16_t result  = a + hl_content;

    addition_flags(a, hl_content, 0);

    a = result;

    cycles += 7;
    pc += 1;
}

// Add immediate to A
void CPU_8080::ADI(uint8_t data)
{
    uint16_t result  = a + data;

    addition_flags(a, data, 0);

    a = (uint8_t) (result & 0xff);

    cycles += 7;
    pc += 2;
}

// Subtract immediate from A
void CPU_8080::SUI(uint8_t data)
{
    uint8_t twos_complement_data =
        -(unsigned int)data;
    uint16_t result  = a + twos_complement_data;
    /* uint16_t result  = a - data; */

    //subtraction_flags(a, data, 0);

    cc.cy = ( (result & 0x100) == 0 );
    cc.z = ((result & 0xff) == 0);
    cc.s = ((result & 0x80) != 0);
    cc.p = Parity(result & 0xff);

    a = (uint8_t) (result & 0xff);

    cycles += 7;
    pc += 2;
}

// Subtract immediate from A with borrow
void CPU_8080::SBI(uint8_t data)
{
    uint16_t data_plus_carry = data + cc.cy;
    uint8_t twos_complement_data_plus_carry =
        -(unsigned int)data_plus_carry;
    uint16_t result  = a + twos_complement_data_plus_carry;

    cc.cy = (result & 0x100) == 0;
    cc.z = ((result & 0xff) == 0);
    cc.s = ((result & 0x80) != 0);
    cc.p = Parity(result & 0xff);

    a = (uint8_t) (result & 0xff);

    cycles += 7;
    pc += 2;
}
