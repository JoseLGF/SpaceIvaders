/*
 * Implementations for the CPU instructions in the call group.
 *
 */

#include "i8080.h"

// Unconditional call
// The address to be called are encoded in the two bytes
// following the call instruction
void CPU_8080::CALL(uint8_t hi, uint8_t lo)
{
    uint16_t ret = pc+2;
    memory[sp-1] = (ret >> 8) & 0xff;
    memory[sp-2] = (ret & 0xff);
    sp = sp - 2;
    pc = (hi << 8) | lo;
}

void CPU_8080::RET()
{
    pc = memory[sp] | (memory[sp+1] << 8);
    sp += 2;
}
