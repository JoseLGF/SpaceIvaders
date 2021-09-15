/*
 * Implementations for the CPU instructions in the branch group.
 * This includes inconditional jumps, jump on zero, etc.
 *
 */

#include "i8080.h"

// Jump on condition
// For JMP, simply pass true
void CPU_8080::J_Cond(uint8_t hi, uint8_t lo, bool cond)
{
    if(cond)
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

