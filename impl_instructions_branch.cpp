/*
 * Implementations for the CPU instructions in the branch group.
 * This includes inconditional jumps, jump on zero, etc.
 *
 */

#include "i8080.h"

void CPU_8080::JMP(uint8_t hi, uint8_t lo)
{
    uint16_t address = (hi << 8) | lo;
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
}
