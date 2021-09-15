/*
 * Implementations for the CPU instructions in the add and subtract
 * groups.
 *
 */

#include "i8080.h"

#include <iostream>
#include <string>

// Add D&E to H&L
void CPU_8080::DAD_D()
{
    uint16_t d_and_e = (d << 8) | e;
    uint16_t h_and_l = (h << 8) | l;
    uint32_t result  = d_and_e + h_and_l;
    h = ( (result >> 8) & 0xff );
    l = result & 0xff;

    cc.cy = ( (result & 0x10000) != 0 );

    cycles += 10;
    pc += 1;
}

// Add memory to A
void CPU_8080::ADD_M()
{
    uint16_t address = (h << 8) | l;
    uint8_t hl_content = MemoryRead(address);
    uint16_t result  = a + hl_content;

    cc.cy = ( (result & 0x100) != 0 );
    /* cc.z = (result == 0); */
    cc.z = ((result & 0xff) == 0);
    cc.s = ((result & 0x80) != 0);
    cc.p = Parity(result & 0xff);
    //cc.ac

    a = result;

    cycles += 7;
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

    cycles += 10;
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

    cycles += 10;
    pc += 1;
}

// Add immediate to A
void CPU_8080::ADI(uint8_t data)
{
    uint16_t result  = a + data;

    cc.cy = ( (result & 0x100) != 0 );
    cc.z = ((result & 0xff) == 0);
    cc.s = ((result & 0x80) != 0);
    /* cc.p = Parity(result); */
    cc.p = Parity(result & 0xff);

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

    cc.cy = ( (result & 0x100) == 0 );
    /* cc.z = (result == 0); */
    cc.z = ((result & 0xff) == 0);
    cc.s = ((result & 0x80) != 0);
    /* cc.p = Parity(result); */
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
    /* cc.z = (result == 0); */
    cc.z = ((result & 0xff) == 0);
    cc.s = ((result & 0x80) != 0);
    /* cc.p = Parity(result); */
    cc.p = Parity(result & 0xff);

    a = (uint8_t) (result & 0xff);

    cycles += 7;
    pc += 2;
}
