/*
 * Implementations for the CPU instructions in the add and subtract
 * groups.
 *
 */

#include "i8080.h"

#include <iostream>

// Add D&E to H&L
void CPU_8080::DAD_D()
{
    uint16_t d_and_e = (d << 8) | e;
    uint16_t h_and_l = (h << 8) | l;
    uint32_t result  = d_and_e + h_and_l;
    h = ( (result >> 8) & 0xff );
    l = result & 0xff;

    cc.cy = ( (result & 0x10000) != 0 );

    pc += 1;
}

// Add B&C to H&L
void CPU_8080::DAD_B()
{
    uint16_t b_and_c = (b << 8) | c;
    uint16_t h_and_l = (h << 8) | l;
    uint32_t result  = b_and_c + h_and_l;
    h = ( (result >> 8) & 0xff );
    l = result & 0xff;

    cc.cy = ( (result & 0x10000) != 0 );

    pc += 1;
}


// Add H&L to H&L
void CPU_8080::DAD_H()
{
    uint16_t h_and_l = (h << 8) | l;
    uint32_t result  = h_and_l + h_and_l;
    h = ( (result >> 8) & 0xff );
    l = result & 0xff;

    cc.cy = ( (result & 0x10000) != 0 );

    pc += 1;
}
