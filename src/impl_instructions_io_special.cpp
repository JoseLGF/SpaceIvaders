/*
 * Implementation for instructions that fall under the IO and special
 * categories
 */

#include "i8080.h"

void CPU_8080::OUT(uint8_t byte)
{
    // Will need to re-implement later!
    (void) byte;
    pc += 2;
    cycles += 10;
}

void CPU_8080::IN(uint8_t device_number)
{
    // Will need to re-implement later!
    (void) device_number;
    pc += 2;
    cycles += 10;
}

void CPU_8080::EI()
{
    int_enable = true;
    pc += 1;
    cycles += 4;
}

// Set carry
void CPU_8080::STC()
{
    cc.cy = true;
    pc += 1;
    cycles += 4;
}
