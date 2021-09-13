#include "i8080.h"
#include <iostream>
#include <fstream>

void CPU_8080::Initialize()
{
    // Initialize registers, stacks, pc and flags
    a  = 0;
    b  = 0;
    c  = 0;
    d  = 0;
    e  = 0;
    h  = 0;
    l  = 0;
    sp = 0;
    pc = 0;
    cc = {0,0,0,0,0,0};
    int_enable = 0;

    for(int i=0; i<0x4000; i++)
    {
        memory[i] = 0;
    }

    instructions_executed = 0;
}


void CPU_8080::LoadRom()
{
    std::cout << "Loading file invaders..." << std::endl;

    FILE * pFile;
    long lSize;
    unsigned char * buffer;
    size_t result;

    pFile = fopen ( "invaders" , "rb" );
    if (pFile==NULL) {fputs ("File error",stderr); exit (1);}

    // obtain file size:
    fseek (pFile , 0 , SEEK_END);
    lSize = ftell (pFile);
    rewind (pFile);

    // allocate memory to contain the whole file:
    buffer = (unsigned char*) malloc (sizeof(char)*lSize);
    if (buffer == NULL) {fputs ("Memory error",stderr); exit (2);}

    // copy the file into the buffer:
    result = fread (buffer,1,lSize,pFile);
    if (result != lSize) {fputs ("Reading error",stderr); exit (3);}

    /* the whole file is now loaded in the memory buffer. */


    // transfer the contents of buffer to the memory starting
    // at address 0x0
    for(int i = 0; i < lSize; ++i)
    {
        if(i<10){
            printf("%02x \n", buffer[i]);
        }
        memory[i] = buffer[i];
    }

    // terminate
    fclose (pFile);
    free(buffer);
}

void CPU_8080::PrintState()
{
    std::cout << "======= CPU STATUS =======" << std::endl;
    std::cout << "CPU Registers:" << std::endl;
    std::cout << "A : " << std::hex << (unsigned int) a << std::endl;
    std::cout << "B : " << std::hex << (unsigned int) b << std::endl;
    std::cout << "C : " << std::hex << (unsigned int) c << std::endl;
    std::cout << "D : " << std::hex << (unsigned int) d << std::endl;
    std::cout << "E : " << std::hex << (unsigned int) e << std::endl;
    std::cout << "H : " << std::hex << (unsigned int) h << std::endl;
    std::cout << "L : " << std::hex << (unsigned int) l << std::endl;
    std::cout << "SP: " << std::hex << (unsigned int) sp << std::endl;
    std::cout << "PC: " << std::hex << (unsigned int) pc << std::endl;
    std::cout << "Processor Flags:" << std::endl;
    std::cout << "Z : " << std::hex << (unsigned int) cc.z << std::endl;
    std::cout << "S : " << std::hex << (unsigned int) cc.s << std::endl;
    std::cout << "P : " << std::hex << (unsigned int) cc.p << std::endl;
    std::cout << "CY: " << std::hex << (unsigned int) cc.cy << std::endl;
    std::cout << "AC: " << std::hex << (unsigned int) cc.ac << std::endl;
    std::cout << "Instructions executed: " << std::dec << instructions_executed << std::endl;
    std::cout << "==========================" << std::endl;
}

void CPU_8080::UnimplementedInstruction(uint8_t opcode)
{
    std::cout << "Found unimplemented instruction: 0x"
        << std::hex << (unsigned int) opcode << std::endl;
    halted = true;
}

void CPU_8080::EmulateCycles(uint32_t num_cycles)
{
    cycles = 0;
    while(cycles < num_cycles)
    {
        RegularInstruction();
    }
}

void CPU_8080::RegularInstruction()
{
    // Check PC boundary
    if (pc > 0x3fff)
    {
        std::cout << "Error, PC out of bounds" << std::endl;
        halted = true;
        return;
    }
    // Fetch opcode
    uint8_t opcode = memory[pc];
    ExecuteInstruction(opcode);
}

void CPU_8080::Interrupt(uint8_t opcode)
{
    // If interrupts are enabled, service the request
    if (int_enable)
    {
        ExecuteInstruction(opcode);
    }
}

void CPU_8080::ExecuteInstruction(uint8_t opcode)
{
    // Decode and execute instruction
    switch(opcode)
    {
        case 0x00:      NOP ();                           break;
        case 0x01:    LXI_B (memory[pc+2], memory[pc+1]); break;
        case 0x02:   STAX_B ();                           break;
        case 0x03:    INX_B ();                           break;
        case 0x05:    DCR_B ();                           break;
        case 0x06:    MVI_B (memory[pc+1]);               break;
        case 0x09:    DAD_B ();                           break;
        case 0x0a:   LDAX_B ();                           break;
        case 0x0d:    DCR_C ();                           break;
        case 0x0e:    MVI_C (memory[pc+1]);               break;
        case 0x0f:      RRC ();                           break;
        case 0x11:    LXI_D (memory[pc+2], memory[pc+1]); break;
        case 0x13:    INX_D ();                           break;
        case 0x19:    DAD_D ();                           break;
        case 0x29:    DAD_H ();                           break;
        case 0x1a:   LDAX_D ();                           break;
        case 0x21:    LXI_H (memory[pc+2], memory[pc+1]); break;
        case 0x23:    INX_H ();                           break;
        case 0x26:    MVI_H (memory[pc+1]);               break;
        case 0x2e:    MVI_L (memory[pc+1]);               break;
        case 0x31:   LXI_SP (memory[pc+2], memory[pc+1]); break;
        case 0x32:      STA (memory[pc+2], memory[pc+1]); break;
        case 0x35:    DCR_M ();                           break;
        case 0x36:    MVI_M (memory[pc+1]);               break;
        case 0x37:      STC ();                           break;
        case 0x3a:      LDA (memory[pc+2], memory[pc+1]); break;
        case 0x3d:    DCR_A ();                           break;
        case 0x3e:    MVI_A (memory[pc+1]);               break;
        case 0x46:  MOV_B_M ();                           break;
        case 0x4f:  MOV_C_A ();                           break;
        case 0x56:  MOV_D_M ();                           break;
        case 0x57:  MOV_D_A ();                           break;
        case 0x5e:  MOV_E_M ();                           break;
        case 0x5f:  MOV_E_A ();                           break;
        case 0x66:  MOV_H_M ();                           break;
        case 0x67:  MOV_H_A ();                           break;
        case 0x6f:  MOV_L_A ();                           break;
        case 0x79:  MOV_A_C ();                           break;
        case 0x7a:  MOV_A_D ();                           break;
        case 0x7b:  MOV_A_E ();                           break;
        case 0x7c:  MOV_A_H ();                           break;
        case 0x7d:  MOV_A_L ();                           break;
        case 0x77:  MOV_M_A ();                           break;
        case 0x7e:  MOV_A_M ();                           break;
        case 0xaf:    XRA_A ();                           break;
        case 0xa7:    ANA_A ();                           break;
        case 0xb0:    ORA_B ();                           break;
        case 0xb6:    ORA_M ();                           break;
        case 0xc0:      RNZ ();                           break;
        case 0xc1:    POP_B ();                           break;
        case 0xc2:      JNZ (memory[pc+2], memory[pc+1]); break;
        case 0xc3:      JMP (memory[pc+2], memory[pc+1]); break;
        case 0xc5:   PUSH_B ();                           break;
        case 0xc6:      ADI (memory[pc+1]);               break;
        case 0xc7:      RST (0x0);                        break;
        case 0xc9:      RET ();                           break;
        case 0xca:       JZ (memory[pc+2], memory[pc+1]); break;
        case 0xcd:     CALL (memory[pc+2], memory[pc+1]); break;
        case 0xcf:      RST (0x1);                        break;
        case 0xc8:       RZ ();                           break;
        case 0xd1:    POP_D ();                           break;
        case 0xd2:      JNC (memory[pc+2], memory[pc+1]); break;
        case 0xd3:      OUT (memory[pc+1]);               break;
        case 0xd5:   PUSH_D ();                           break;
        case 0xd7:      RST (0x2);                        break;
        case 0xd8:       RC ();                           break;
        case 0xda:       JC (memory[pc+2], memory[pc+1]); break;
        case 0xdb:       IN (memory[pc+1]);               break;
        case 0xdf:      RST (0x3);                        break;
        case 0xeb:     XCHG ();                           break;
        case 0xe1:    POP_H ();                           break;
        case 0xe5:   PUSH_H ();                           break;
        case 0xe6:      ANI (memory[pc+1]);               break;
        case 0xe7:      RST (0x4);                        break;
        case 0xef:      RST (0x5);                        break;
        case 0xf1:  POP_PSW ();                           break;
        case 0xf5: PUSH_PSW ();                           break;
        case 0xf7:      RST (0x6);                        break;
        case 0xfe:      CPI (memory[pc+1]);               break;
        case 0xfb:       EI ();                           break;
        case 0xff:      RST (0x7);                        break;
        default  : UnimplementedInstruction(opcode);      break;
    }

    instructions_executed++;
}

bool CPU_8080::Running()
{
    return !halted;
}

void CPU_8080::DumpMemory()
{
    using namespace std;
    ofstream wf("memdump", ios::out | ios::binary);
    if(!wf) {
        cout << "Error, could not write memory dump file." << endl;
        return;
    }

    wf.write(reinterpret_cast<char*>(&memory), sizeof(memory));
    wf.close();
    cout << "Dumped memory to memdump file." << endl;

}

// Even parity lookup table
bool CPU_8080::Parity(uint8_t byte)
{

    bool parity_table[] = {
      1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1,
      0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0,
      0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0,
      1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1,
      0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0,
      1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1,
      1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1,
      0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0,
      0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0,
      1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1,
      1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1,
      0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0,
      1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1,
      0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0,
      0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0,
      1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1
    };
    return parity_table[byte];

    /* bool parity = true; */

    /* for(int i=0; i<8; i++) */
    /* { */
    /*     if(byte & 0x01) */
    /*     { */
    /*         parity = !parity; */
    /*     } */
    /*     parity = (parity >> 1) & 0x7f; */
    /* } */

    /* return parity; */
}

uint8_t CPU_8080::MemoryRead(uint16_t address)
{
    /* Implement RAM mirroring by dropping the two MSBs from the */
    /* provided address */
    return memory[address & 0x3fff];
}

void CPU_8080::MemoryWrite(uint16_t address, uint8_t data)
{
    // Signal an error if the address points to a ROM area
    // 0x0000 - 0x1fff : ROM
    if ((address & 0x3fff) < 0x2000)
    {
        std::cout << "Error, attempting to write to ROM" << std::endl;
        PrintState();
        halted = true;
        return;
    }

    memory[address & 0x3fff] = data;
}

void CPU_8080::WriteMemoryAt(uint16_t address, uint8_t data)
{
    memory[address] = data;
}
uint8_t CPU_8080::ReadMemoryAt(uint16_t address)
{
    return memory[address];
}


uint16_t CPU_8080::Get_pc() { return pc; }
uint16_t CPU_8080::Get_sp() { return sp; }

uint8_t CPU_8080::Get_a() { return a; }
uint8_t CPU_8080::Get_b() { return b; }
uint8_t CPU_8080::Get_c() { return c; }
uint8_t CPU_8080::Get_d() { return d; }
uint8_t CPU_8080::Get_e() { return e; }
uint8_t CPU_8080::Get_h() { return h; }
uint8_t CPU_8080::Get_l() { return l; }

bool CPU_8080::Get_cy() { return cc.cy; }
bool CPU_8080::Get_ac() { return cc.ac; }
bool CPU_8080::Get_z() { return cc.z; }
bool CPU_8080::Get_s() { return cc.s; }
bool CPU_8080::Get_p() { return cc.p; }

void CPU_8080::Set_ac(bool bit) { cc.ac = bit; }
void CPU_8080::Set_cy(bool bit) { cc.cy = bit; }
void CPU_8080::Set_z(bool bit) { cc.z = bit; }
void CPU_8080::Set_s(bool bit) { cc.s = bit; }
void CPU_8080::Set_p(bool bit) { cc.p = bit; }
void CPU_8080::Set_a(uint8_t data) { a = data; }
void CPU_8080::Set_b(uint8_t data) { b = data; }
void CPU_8080::Set_c(uint8_t data) { c = data; }
void CPU_8080::Set_d(uint8_t data) { d = data; }
void CPU_8080::Set_e(uint8_t data) { e = data; }
void CPU_8080::Set_h(uint8_t data) { h = data; }
void CPU_8080::Set_l(uint8_t data) { l = data; }

void CPU_8080::Set_sp(uint16_t address) { sp = address; }
void CPU_8080::Set_pc(uint16_t address) { pc = address; }

