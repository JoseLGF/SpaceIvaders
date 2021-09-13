/*
 * Implementations for the CPU instructions in the branch group.
 * This includes inconditional jumps, jump on zero, etc.
 *
 */

#include "i8080.h"

void CPU_8080::JMP(uint8_t hi, uint8_t lo)
{
    uint16_t address = (hi << 8) | lo;
    cycles += 10;
    pc = address;
}

// Jump if no zero
void CPU_8080::JNZ(uint8_t hi, uint8_t lo)
{
    if(cc.z == 0)
    {
        uint16_t address = (hi << 8) | lo;
        pc = address;
    }
    else
    {
        pc += 3;
    }
    cycles += 10;
}

// Jump on zero
void CPU_8080::JZ(uint8_t hi, uint8_t lo)
{
    if(cc.z == 1)
    {
        uint16_t address = (hi << 8) | lo;
        pc = address;
    }
    else
    {
        pc += 3;
    }
    cycles += 10;
}

// Jump on minus
void CPU_8080::JM(uint8_t hi, uint8_t lo)
{
    if(cc.s == 1)
    {
        uint16_t address = (hi << 8) | lo;
        pc = address;
    }
    else
    {
        pc += 3;
    }
    cycles += 10;
}

// Jump on carry
void CPU_8080::JC(uint8_t hi, uint8_t lo)
{
    if(cc.cy == 1)
    {
        uint16_t address = (hi << 8) | lo;
        pc = address;
    }
    else
    {
        pc += 3;
    }
    cycles += 10;
}

// Jump on no carry
void CPU_8080::JNC(uint8_t hi, uint8_t lo)
{
    if(cc.cy == 0)
    {
        uint16_t address = (hi << 8) | lo;
        pc = address;
    }
    else
    {
        pc += 3;
    }
    cycles += 10;
}
