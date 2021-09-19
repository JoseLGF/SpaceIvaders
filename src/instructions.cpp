/*
 * Implementations for the CPU instructions in i8080 cpu
 *
 */

#include "i8080.h"

#include <iostream>
#include <string>

// Add r1&r2 to H&L
void CPU_8080::DAD(uint8_t& r1, uint8_t& r2)
{
    uint16_t r1_and_r2 = (r1 << 8) | r2;
    uint16_t h_and_l = (h << 8) | l;
    uint32_t result  = (uint32_t)r1_and_r2 + (uint32_t)h_and_l;
    h = ( (result >> 8) & 0xff );
    l = result & 0xff;

    cc.cy = (result > 0xffff);

    cycles += 10;
    pc += 1;
}

// Add SP to H&L
void CPU_8080::DAD_SP()
{
    uint16_t h_and_l = (h << 8) | l;
    uint32_t result  = (uint32_t)sp + (uint32_t)h_and_l;
    h = ( (result >> 8) & 0xff );
    l = result & 0xff;

    cc.cy = (result > 0xffff);

    cycles += 10;
    pc += 1;
}

// Add memory to A
void CPU_8080::ADD_M()
{
    uint16_t address = (h << 8) | l;
    uint8_t hl_content = MemoryRead(address);
    uint16_t result  = a + hl_content;

    addition_flags(a, hl_content, 0);

    a = result;

    cycles += 7;
    pc += 1;
}

// Add memory to A with carry
void CPU_8080::ADC_M()
{
    uint16_t address = (h << 8) | l;
    uint8_t hl_content = MemoryRead(address);
    uint16_t result  = a + hl_content + cc.cy;

    addition_flags(a, hl_content, cc.cy);

    a = result;

    cycles += 7;
    pc += 1;
}

// Add register to A
void CPU_8080::ADD_r(uint8_t& r)
{
    uint16_t result  = a + r;

    addition_flags(a, r, 0);

    a = result;

    cycles += 4;
    pc += 1;
}

// Add register to A with carry
void CPU_8080::ADC_r(uint8_t& r)
{
    uint16_t result  = a + r + cc.cy;

    addition_flags(a, r, cc.cy);

    a = result;

    cycles += 4;
    pc += 1;
}

// Add immediate to A
void CPU_8080::ADI(uint8_t data)
{
    uint16_t result  = a + data;

    addition_flags(a, data, 0);

    a = (uint8_t) (result & 0xff);

    cycles += 7;
    pc += 2;
}

// Add immediate to A with carry
void CPU_8080::ACI(uint8_t data)
{
    uint16_t result  = a + data + cc.cy;

    addition_flags(a, data, cc.cy);

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

    subtraction_flags(a, data, 0);

    /* cc.cy = ( (result & 0x100) == 0 ); */
    /* cc.z = ((result & 0xff) == 0); */
    /* cc.s = ((result & 0x80) != 0); */
    /* cc.p = Parity(result & 0xff); */

    a = (uint8_t) (result & 0xff);

    cycles += 7;
    pc += 2;
}

// Subtract register from A
void CPU_8080::SUB_r(uint8_t& r)
{
    uint8_t twos_complement_r =
        -(unsigned int)r;
    uint16_t result  = a + twos_complement_r;
    /* uint16_t result  = a - data; */

    subtraction_flags(a, r, 0);

    a = (uint8_t) (result & 0xff);

    cycles += 4;
    pc += 1;
}

// Subtract register from A with borrow
void CPU_8080::SBB_r(uint8_t& r)
{
    uint8_t twos_complement_r =
        -(unsigned int)r;
    uint16_t result  = a - r - (uint8_t)cc.cy;

    subtraction_flags(a, r, cc.cy);

    a = (uint8_t) (result & 0xff);

    cycles += 4;
    pc += 1;
}

// Subtract memory from A
void CPU_8080::SUB_M()
{
    uint16_t address = (h << 8) | l;
    uint8_t mem = MemoryRead(address);
    uint8_t twos_complement_mem =
        -(unsigned int)mem;
    uint16_t result  = a + twos_complement_mem;

    subtraction_flags(a, mem, 0);

    a = (uint8_t) (result & 0xff);

    cycles += 7;
    pc += 1;
}

// Subtract memory from A with borrow
void CPU_8080::SBB_M()
{
    uint16_t address = (h << 8) | l;
    uint8_t mem = MemoryRead(address);
    uint16_t result  = a - mem - cc.cy;

    subtraction_flags(a, mem, cc.cy);

    a = (uint8_t) (result & 0xff);

    cycles += 7;
    pc += 1;
}

// Subtract immediate from A with borrow
void CPU_8080::SBI(uint8_t data)
{
    uint16_t data_plus_carry = data + cc.cy;
    uint8_t twos_complement_data_plus_carry =
        -(unsigned int)data_plus_carry;
    uint16_t result  = a + twos_complement_data_plus_carry;

    cc.cy = (result & 0x100) == 0;
    cc.z = ((result & 0xff) == 0);
    cc.s = ((result & 0x80) != 0);
    cc.p = Parity(result & 0xff);

    a = (uint8_t) (result & 0xff);

    cycles += 7;
    pc += 2;
}

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

// Unconditional call
// The address to be called are encoded in the two bytes
// following the call instruction
void CPU_8080::CALL(uint8_t hi, uint8_t lo)
{

    uint16_t address = (hi << 8) | lo;
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

void CPU_8080::PCHL()
{
    pc = (h << 8) | l;
    cycles += 5;
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

void CPU_8080::INR_r(uint8_t& r)
{
    uint8_t result = r + 1;

    cc.z = (result == 0);
    cc.s = ((result & 0x80) != 0);
    cc.p = Parity(result);
    // cc.c = Unaffected for this instruction

    r = result;
    pc += 1;
    cycles += 5;
}

void CPU_8080::INR_M()
{
    uint16_t address = (h << 8) | l;
    uint8_t num = MemoryRead(address);
    num++;
    MemoryWrite(address, num);

    cc.z = (num == 0);
    cc.s = ((num & 0x80) != 0);
    cc.p = Parity(num);

    pc += 1;
    cycles += 10;
}

void CPU_8080::DCR_r(uint8_t& r)
{
    uint8_t result = r - 1;

    cc.z = (result == 0);
    cc.s = ((result & 0x80) != 0);
    cc.p = Parity(result);
    // cc.c = Unaffected for this instruction

    r = result;
    pc += 1;
    cycles += 5;
}

// Increment register pair
void CPU_8080::INX(uint8_t& r1, uint8_t& r2)
{
    uint16_t num = (r1 << 8) | r2;
    num++;
    r1 = (uint8_t) ((num >> 8) & 0xff);
    r2 = (uint8_t) (num & 0xff);
    pc += 1;
    cycles += 5;
}

// Increment stack pointer
void CPU_8080::INX_SP()
{
    sp++;
    pc += 1;
    cycles += 5;
}

// Decrement HL register pair
void CPU_8080::DCX_H()
{
    uint16_t num = (h << 8) | l;
    num--;
    h = (uint8_t) ((num >> 8) & 0xff);
    l = (uint8_t) (num & 0xff);
    pc += 1;
    cycles += 5;
}

// Decrement Stack pointer
void CPU_8080::DCX_SP()
{
    sp--;
    pc += 1;
    cycles += 5;
}

// Decrement BC register pair
void CPU_8080::DCX_B()
{
    uint16_t num = (b << 8) | c;
    num--;
    b = (uint8_t) ((num >> 8) & 0xff);
    c = (uint8_t) (num & 0xff);
    pc += 1;
    cycles += 5;
}

// Decrement DE register pair
void CPU_8080::DCX_D()
{
    uint16_t num = (d << 8) | e;
    num--;
    d = (uint8_t) ((num >> 8) & 0xff);
    e = (uint8_t) (num & 0xff);
    pc += 1;
    cycles += 5;
}


// Decrement memory
void CPU_8080::DCR_M()
{
    uint16_t address = (h << 8) | l;
    uint8_t num = MemoryRead(address);
    num--;
    MemoryWrite(address, num);

    cc.z = (num == 0);
    cc.s = ((num & 0x80) != 0);
    cc.p = Parity(num);
    // cc.c = Unaffected for this instruction

    pc += 1;
    cycles += 10;
}

void CPU_8080::OUT(uint8_t device_number)
{
    devices->Write_device(device_number, a);
    pc += 2;
    cycles += 10;
}

void CPU_8080::IN(uint8_t device_number)
{
    a = devices->Read_device(device_number);
    pc += 2;
    cycles += 10;
}

void CPU_8080::EI()
{
    int_enable = true;
    pc += 1;
    cycles += 4;
}

void CPU_8080::DI()
{
    int_enable = false;
    pc += 1;
    cycles += 4;
}

// Set carry
void CPU_8080::STC()
{
    cc.cy = true;
    pc += 1;
    cycles += 4;
}

// Complement accumulator
void CPU_8080::CMA()
{
    a = ~a;
    pc += 1;
    cycles += 4;
}

// Complement carry
void CPU_8080::CMC()
{
    cc.cy = !cc.cy;
    pc += 1;
    cycles += 4;
}

void CPU_8080::ANA_r(uint8_t& r)
{
    uint8_t result = a & r;

    logical_flags(result);

    a = result;
    pc += 1;
    cycles += 4;
}

void CPU_8080::ANA_M()
{
    uint16_t address = (h << 8) | l;
    uint8_t hl_content = MemoryRead(address);
    uint8_t result = a & hl_content;

    logical_flags(result);

    a = result;
    pc += 1;
    cycles += 4;
}

// Compare register with A
void CPU_8080::CMP_r(uint8_t& r)
{
    uint8_t result = a - r;

    logical_flags(result);

    cc.cy = (r > a);
    if((a & 0x80) != (r & 0x80))
    {
        cc.cy = !cc.cy;
    }

    pc += 1;
    cycles += 4;
}

// Compare memory with A
void CPU_8080::CMP_M()
{
    uint16_t address = (h << 8) | l;
    uint8_t hl_content = MemoryRead(address);
    uint8_t result = a - hl_content;

    logical_flags(result);

    cc.cy = (hl_content > a);
    if((a & 0x80) != (hl_content & 0x80))
    {
        cc.cy = !cc.cy;
    }

    pc += 1;
    cycles += 7;
}

void CPU_8080::XRA_r(uint8_t& r)
{
    uint8_t result = a ^ r;

    logical_flags(result);

    a = result;
    pc += 1;
    cycles += 4;
}

void CPU_8080::ORA_r(uint8_t& r)
{
    uint8_t result = a | r;

    logical_flags(result);

    a = result;
    pc += 1;
    cycles += 4;
}

// ORA M, Or memory with A
void CPU_8080::ORA_M()
{
    uint16_t address = (h << 8) | l;
    uint8_t hl_content = MemoryRead(address);
    uint8_t result = a | hl_content;

    logical_flags(result);

    a = result;
    pc += 1;
    cycles += 7;
}

// Xor memory with A
void CPU_8080::XRA_M()
{
    uint16_t address = (h << 8) | l;
    uint8_t hl_content = MemoryRead(address);
    uint8_t result = a ^ hl_content;

    logical_flags(result);

    a = result;
    pc += 1;
    cycles += 7;
}

// Compare immediate with accumulator
void CPU_8080::CPI(uint8_t data)
{
    uint8_t result = a - data;

    cc.z  = (result == 0);
    cc.s  = ((result & 0x80) != 0);
    cc.p  = Parity(result);
    cc.cy = data > a;

    /* a = result; */ //Ouch! This should not happened
    pc += 2;
    cycles += 7;
}

// Rotate A Right
void CPU_8080::RRC()
{
    uint8_t lsb = a & 0x01;
    a = (a >> 1) | (lsb << 7);

    cc.cy = lsb;

    pc += 1;
    cycles += 4;
}

// Rotate A Right through carry
void CPU_8080::RAR()
{
    uint8_t prev_cy = cc.cy;
    uint8_t lsb = a & 0x01;
    a = (a >> 1) | (prev_cy << 7);

    cc.cy = lsb;

    pc += 1;
    cycles += 4;
}

// Rotate A Left through carry
void CPU_8080::RAL()
{
    uint8_t prev_cy = cc.cy;
    uint8_t msb = a & 0x80;
    a = (a << 1) | (prev_cy);

    cc.cy = (msb != 0);

    pc += 1;
    cycles += 4;
}

// Rotate A Left
void CPU_8080::RLC()
{
    uint8_t msb = a & 0x80;
    a = (a << 1) | (msb >> 7);

    cc.cy = (msb != 0);

    pc += 1;
    cycles += 4;
}

// And immediate with A
void CPU_8080::ANI(uint8_t data)
{
    uint8_t result = a & data;

    logical_flags(result);

    a = result;
    pc += 2;
    cycles += 7;
}

// ORI, Or immediate with A
void CPU_8080::ORI(uint8_t data)
{
    uint8_t result = a | data;

    logical_flags(result);

    a = result;
    pc += 2;
    cycles += 7;
}

// XRI, Xor immediate with A
void CPU_8080::XRI(uint8_t data)
{
    uint8_t result = a ^ data;

    logical_flags(result);

    a = result;
    pc += 2;
    cycles += 7;
}

void CPU_8080::MVI(uint8_t& reg, uint8_t data)
{
    reg = data;
    pc += 2;
    cycles += 7;
}

void CPU_8080::MOV_r_r(uint8_t& r1, uint8_t& r2)
{
    r1 = r2;
    pc += 1;
    cycles += 5;
}

void CPU_8080::MOV_r_m(uint8_t& r)
{
    uint16_t address = (h << 8) | l;
    r = MemoryRead(address);
    pc += 1;
    cycles += 7;
}

void CPU_8080::MOV_m_r(uint8_t& r)
{
    uint16_t address = (h << 8) | l;
    MemoryWrite(address, r);
    pc += 1;
    cycles += 7;
}

// Load immediate register pair
void CPU_8080::LXI(uint8_t& r1, uint8_t& r2, uint8_t d1, uint8_t d2)
{
    r1 = d1;
    r2 = d2;
    pc += 3;
    cycles += 10;
}

// Load A indirect from the address pointed to by the register pair
void CPU_8080::LDAX(uint8_t& r1, uint8_t& r2)
{
    uint16_t address = (r1 << 8) | r2;
    a = MemoryRead(address);
    pc += 1;
    cycles += 7;
}

// Store A direct
void CPU_8080::STA(uint8_t byte_h, uint8_t byte_l)
{
    uint16_t address = (byte_h << 8) | byte_l;
    MemoryWrite(address, a);
    pc += 3;
    cycles += 13;
}

// Load A direct
void CPU_8080::LDA(uint8_t byte_h, uint8_t byte_l)
{
    uint16_t address = (byte_h << 8) | byte_l;
    a = MemoryRead(address);
    pc += 3;
    cycles += 13;
}

// Move immediate Memory
void CPU_8080::MVI_M(uint8_t data)
{
    uint16_t address = (h << 8) | l;
    MemoryWrite(address, data);
    pc += 2;
    cycles += 10;
}

// Exchange the contents of register pairs HL and DE
void CPU_8080::XCHG()
{
    uint8_t tmp;
    tmp = h;
    h = d;
    d = tmp;
    tmp = l;
    l = e;
    e = tmp;

    pc += 1;
    cycles += 4;
}


// Store A indirect
void CPU_8080::STAX_B()
{
    uint16_t address = (b << 8) | c;
    MemoryWrite(address, a);
    pc += 1;
    cycles += 7;
}

// Store A indirect
void CPU_8080::STAX_D()
{
    uint16_t address = (d << 8) | e;
    MemoryWrite(address, a);
    pc += 1;
    cycles += 7;
}

// Load HL pair direct
void CPU_8080::LHLD(uint8_t hi, uint8_t lo)
{
    uint16_t address = (hi << 8) | lo;
    l = MemoryRead(address);
    h = MemoryRead(address+1);
    pc += 3;
    cycles += 16;
}

// Store HL pair direct
void CPU_8080::SHLD(uint8_t hi, uint8_t lo)
{
    uint16_t address = (hi << 8) | lo;
    MemoryWrite(address, l);
    MemoryWrite(address+1, h);
    pc += 3;
    cycles += 16;
}

void CPU_8080::NOP()
{
    pc++;
    cycles += 4;
}

// Load immediate stack pointer
void CPU_8080::LXI_SP(uint8_t hi, uint8_t lo)
{
    uint16_t address = (hi << 8) | lo;
    sp = address;
    pc += 3;
    cycles += 10;
}

// Push Register Pair
void CPU_8080::PUSH_rp(uint8_t& r1, uint8_t& r2)
{
    MemoryWrite(sp-1, r1);
    MemoryWrite(sp-2, r2);
    sp = sp - 2;
    pc += 1;
}

// Pop Register Pair
void CPU_8080::POP_rp(uint8_t& r1, uint8_t& r2)
{
    r1 = MemoryRead(sp+1);
    r2 = MemoryRead(sp);
    sp = sp + 2;
    pc += 1;
    cycles += 10;
}

// Push Flags and A on stack
void CPU_8080::PUSH_PSW()
{
    uint8_t flags = 0x00;
    if (cc.cy) { flags |= 0x01; }   // bit 0
                 flags |= 0x02;     // bit 1 always set
    if (cc.p)  { flags |= 0x04; }   // bit 2
                                    // bit 3 always 0
    if (cc.ac) { flags |= 0x10; }   // bit 4
                                    // bit 5 always 0
    if (cc.z)  { flags |= 0x40; }   // bit 6
    if (cc.s)  { flags |= 0x80; }   // bit 7

    MemoryWrite(sp-1, a);
    MemoryWrite(sp-2, flags);
    sp = sp - 2;
    pc += 1;
    cycles += 11;
}

// Pop Flags and A Off Stack
void CPU_8080::POP_PSW()
{
    uint8_t flags = MemoryRead(sp);
    cc.cy = ((flags & 0x01) != 0);   // bit 0
    cc.p  = ((flags & 0x04) != 0);   // bit 2
    cc.ac = ((flags & 0x10) != 0);   // bit 4
    cc.z  = ((flags & 0x40) != 0);   // bit 6
    cc.s  = ((flags & 0x80) != 0);   // bit 7

    a = MemoryRead(sp+1);
    sp = sp + 2;
    pc += 1;
    cycles += 10;
}

// Exchange top of stack, H & L
void CPU_8080::XTHL()
{
    uint8_t tmp;
    // l<->(SP)
    tmp = l;
    l = MemoryRead(sp);
    MemoryWrite(sp, tmp);
    // h<->(SP+1)
    tmp = h;
    h = MemoryRead(sp+1);
    MemoryWrite(sp+1, tmp);

    pc += 1;
    cycles += 18;
}

// HL to stack pointer
void CPU_8080::SPHL()
{
    sp = (h << 8) | l;

    pc += 1;
    cycles += 5;
}

uint8_t CPU_8080::read_rp(uint8_t r1, uint8_t r2)
{
    return (r1 << 8) | r2;
}
