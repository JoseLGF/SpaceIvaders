/*
 * Implementations for the CPU instructions in the stack operations group.
 *
 */

#include "i8080.h"

// Load immediate stack pointer
void CPU_8080::LXI_SP(uint8_t hi, uint8_t lo)
{
    uint16_t address = (hi << 8) | lo;
    sp = address;
    pc += 3;
}
