#include "i8080.h"

void CPU_8080::NOP()
{
    pc++;
    cycles += 4;
}
