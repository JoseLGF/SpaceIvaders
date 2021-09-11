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
}

void CPU_8080::EI()
{
    int_enable = true;
    pc += 1;
}
