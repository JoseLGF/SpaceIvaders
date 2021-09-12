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

// Push Register Pair D & E on stack
void CPU_8080::PUSH_D()
{
    memory[sp-1] = d;
    memory[sp-2] = e;
    sp = sp - 2;
    pc += 1;
}

// Push Register Pair H & L on stack
void CPU_8080::PUSH_H()
{
    memory[sp-1] = h;
    memory[sp-2] = l;
    sp = sp - 2;
    pc += 1;
}

// Pop Register Pair H & L off stack
void CPU_8080::POP_H()
{
    l = memory[sp];
    h = memory[sp+1];
    sp = sp + 2;
    pc += 1;
}

// Push Register Pair B & C on stack
void CPU_8080::PUSH_B()
{
    memory[sp-1] = b;
    memory[sp-2] = c;
    sp = sp - 2;
    pc += 1;
}

// Pop Register Pair B & C off stack
void CPU_8080::POP_B()
{
    c = memory[sp];
    b = memory[sp+1];
    sp = sp + 2;
    pc += 1;
}

// Pop Register Pair D & E off stack
void CPU_8080::POP_D()
{
    e = memory[sp];
    d = memory[sp+1];
    sp = sp + 2;
    pc += 1;
}
