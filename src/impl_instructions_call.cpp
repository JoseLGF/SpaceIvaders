/*
 * Implementations for the CPU instructions in the call, return and
 * restart groups.
 *
 */

#include "i8080.h"

// Unconditional call
// The address to be called are encoded in the two bytes
// following the call instruction
void CPU_8080::CALL(uint8_t hi, uint8_t lo)
{
    uint16_t ret = pc+3;
    MemoryWrite(sp-1 , (ret >> 8) & 0xff);
    MemoryWrite(sp-2 , (ret & 0xff));
    sp = sp - 2;
    pc = (hi << 8) | lo;
    cycles += 17;
}

// Call on condition
void CPU_8080::C_Cond(uint8_t hi, uint8_t lo, bool cond)
{
    if (cond)
    {
        uint16_t ret = pc+3;
        MemoryWrite(sp-1 , (ret >> 8) & 0xff);
        MemoryWrite(sp-2 , (ret & 0xff));
        sp = sp - 2;
        pc = (hi << 8) | lo;
        cycles += 17;
    }
    else
    {
        pc += 3;
        cycles += 11;
    }
}

// Return on condition
void CPU_8080::R_cond(bool cond)
{
    if(cond)
    {
        RET();
        // 10 cycles of RET + 1 cycle of RZ = 11 cycles
        cycles += 1;
    }
    else
    {
        cycles += 5;
        pc ++;
    }
}

void CPU_8080::RET()
{
    pc = MemoryRead(sp) | (MemoryRead(sp+1) << 8);
    sp += 2;
    cycles += 10;
}

void CPU_8080::RST(uint8_t exp)
{
    uint16_t new_pc = ((uint16_t)exp << 3);
    MemoryWrite(sp-1 , (pc >> 8) & 0xff);
    MemoryWrite(sp-2 , (pc & 0xff));
    sp = sp - 2;
    pc = new_pc;
    cycles += 11;
}
