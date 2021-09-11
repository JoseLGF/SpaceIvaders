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
}

// Increment DE register pair
void CPU_8080::INX_D()
{
    uint16_t num = (d << 8) | e;
    num++;
    d = (uint8_t) ((num >> 8) & 0xff);
    e = (uint8_t) (num & 0xff);
    pc += 1;
}

// Decrement B register
void CPU_8080::DCR_B()
{
    if(b==0)
    {
        std::cout << "Warning, possible underflow of B register."
            << std::endl;
    }

    uint8_t result = b - 1;

    cc.z = (result == 0);
    cc.s = ((result & 0x80) != 0);
    cc.p = Parity(result);
    // cc.c = Unaffected for this instruction

    b = result;
    pc += 1;
}
