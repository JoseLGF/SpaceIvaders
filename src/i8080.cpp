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


void CPU_8080::LoadRom(std::string filename)
{
    std::cout << "Loading file " << filename << std::endl;

    FILE * pFile;
    long lSize;
    unsigned char * buffer;
    size_t result;

    const char * fname = filename.c_str();
    pFile = fopen ( fname, "rb" );
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
        int_enable = false;
        ExecuteInstruction(opcode);
    }
}

void CPU_8080::ExecuteInstruction(uint8_t opcode)
{
#define BYTE1 memory[pc+1]
#define BYTE2 memory[pc+2]
    // Decode and execute instruction
    switch(opcode)
    {
        case 0x00:      NOP ();                                   break;
        case 0x01:      LXI (b, c, BYTE2, BYTE1);                 break;
        case 0x02:   STAX_B ();                                   break;
        case 0x03:      INX (b, c);                               break;
        case 0x04:    INR_r (b);                                  break;
        case 0x05:    DCR_r (b);                                  break;
        case 0x06:      MVI (b, BYTE1);                           break;
        case 0x07:      RLC ();                                   break;
        case 0x09:      DAD (b, c);                               break;
        case 0x0a:     LDAX (b, c);                               break;
        case 0x0d:    DCR_r (c);                                  break;
        case 0x0e:      MVI (c, BYTE1);                           break;
        case 0x0f:      RRC ();                                   break;
        case 0x11:      LXI (d, e, BYTE2, BYTE1);                 break;
        case 0x13:      INX (d, e);                               break;
        case 0x14:    INR_r (d);                                  break;
        case 0x15:    DCR_r (d);                                  break;
        case 0x16:      MVI (d, BYTE1);                           break;
        case 0x19:      DAD (d, e);                               break;
        case 0x29:      DAD (h, l);                               break;
        case 0x1a:     LDAX (d, e);                               break;
        case 0x1f:      RAR ();                                   break;
        case 0x21:      LXI (h, l, BYTE2, BYTE1);                 break;
        case 0x22:     SHLD (BYTE2, BYTE1);                       break;
        case 0x23:      INX (h, l);                               break;
        case 0x26:      MVI (h, BYTE1);                           break;
        case 0x2a:     LHLD (BYTE2, BYTE1);                       break;
        case 0x2b:    DCX_H ();                                   break;
        case 0x2e:      MVI (l, BYTE1);                           break;
        case 0x31:   LXI_SP (BYTE2, BYTE1);                       break;
        case 0x32:      STA (BYTE2, BYTE1);                       break;
        case 0x35:    DCR_M ();                                   break;
        case 0x36:    MVI_M (BYTE1);                              break;
        case 0x37:      STC ();                                   break;
        case 0x3a:      LDA (BYTE2, BYTE1);                       break;
        case 0x3c:    INR_r (a);                                  break;
        case 0x3d:    DCR_r (a);                                  break;
        case 0x3e:      MVI (a, BYTE1);                           break;
        case 0x46:  MOV_r_m (b);                                  break;
        case 0x47:  MOV_r_r (b, a);                               break;
        case 0x4e:  MOV_r_m (c);                                  break;
        case 0x4f:  MOV_r_r (c, a);                               break;
        case 0x56:  MOV_r_m (d);                                  break;
        case 0x57:  MOV_r_r (d, a);                               break;
        case 0x5e:  MOV_r_m (e);                                  break;
        case 0x5f:  MOV_r_r (e, a);                               break;
        case 0x61:  MOV_r_r (h, c);                               break;
        case 0x66:  MOV_r_m (h);                                  break;
        case 0x67:  MOV_r_r (h, a);                               break;
        case 0x68:  MOV_r_r (l, b);                               break;
        case 0x6f:  MOV_r_r (l, a);                               break;
        case 0x70:  MOV_m_r (b);                                  break;
        case 0x78:  MOV_r_r (a, b);                               break;
        case 0x79:  MOV_r_r (a, c);                               break;
        case 0x7a:  MOV_r_r (a, d);                               break;
        case 0x7b:  MOV_r_r (a, e);                               break;
        case 0x7c:  MOV_r_r (a, h);                               break;
        case 0x7d:  MOV_r_r (a, l);                               break;
        case 0x77:  MOV_m_r (a);                                  break;
        case 0x7e:  MOV_r_m (a);                                  break;
        case 0x86:    ADD_M ();                                   break;
        case 0xaf:    XRA_r (a);                                  break;
        case 0xa7:    ANA_r (a);                                  break;
        case 0xb0:    ORA_r (b);                                  break;
        case 0xb4:    ORA_r (h);                                  break;
        case 0xb6:    ORA_M ();                                   break;
        case 0xc0:   R_cond (!cc.z);                              break;
        case 0xc1:   POP_rp (b, c);                               break;
        case 0xc2:   J_Cond (BYTE2, BYTE1, !cc.z);                break;
        case 0xc3:   J_Cond (BYTE2, BYTE1, true);                 break;
        case 0xc4:   C_Cond (BYTE2, BYTE1, (!cc.z));              break;
        case 0xc5:  PUSH_rp (b, c);                               break;
        case 0xc6:      ADI (BYTE1);                              break;
        case 0xc7:      RST (0x0);                                break;
        case 0xc9:      RET ();                                   break;
        case 0xca:   J_Cond (BYTE2, BYTE1, cc.z);                 break;
        case 0xcc:   C_Cond (BYTE2, BYTE1, cc.z);                 break;
        case 0xcd:     CALL (BYTE2, BYTE1);                       break;
        case 0xcf:      RST (0x1);                                break;
        case 0xc8:   R_cond (cc.z);                               break;
        case 0xd0:   R_cond (!cc.cy);                             break;
        case 0xd1:   POP_rp (d, e);                               break;
        case 0xd2:   J_Cond (BYTE2, BYTE1, !cc.cy);               break;
        case 0xd3:      OUT (BYTE1);                              break;
        case 0xd5:  PUSH_rp (d, e);                               break;
        case 0xd6:      SUI (BYTE1);                              break;
        case 0xd7:      RST (0x2);                                break;
        case 0xd8:   R_cond (cc.cy);                              break;
        case 0xda:   J_Cond (BYTE2, BYTE1, cc.cy);                break;
        case 0xdb:       IN (BYTE1);                              break;
        case 0xde:      SBI (BYTE1);                              break;
        case 0xdf:      RST (0x3);                                break;
        case 0xeb:     XCHG ();                                   break;
        case 0xe1:   POP_rp (h, l);                               break;
        case 0xe5:  PUSH_rp (h, l);                               break;
        case 0xe6:      ANI (BYTE1);                              break;
        case 0xe7:      RST (0x4);                                break;
        case 0xef:      RST (0x5);                                break;
        case 0xf1:  POP_PSW ();                                   break;
        case 0xf5: PUSH_PSW ();                                   break;
        case 0xf6:      ORI (BYTE1);                              break;
        case 0xf7:      RST (0x6);                                break;
        case 0xfa:   J_Cond (BYTE2, BYTE1, cc.s);                 break;
        case 0xfe:      CPI (BYTE1);                              break;
        case 0xfb:       EI ();                                   break;
        case 0xff:      RST (0x7);                                break;
        default  : UnimplementedInstruction(opcode);              break;
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

/* Initial implementations of R/W used for unit testing */
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


void CPU_8080::Connect_io_dev(Io_devices* l_devices)
{
    devices = l_devices;
}

void CPU_8080::logical_flags(uint8_t result)
{
    // Sets the flags according to the result after a logical operation
    cc.z  = (result == 0);
    cc.s  = ((result & 0x80) != 0);
    cc.p  = Parity(result);
    cc.cy = 0; // Reset according to programmer's manual.
}

void CPU_8080::addition_flags(uint8_t i_a, uint8_t i_b, uint8_t cy)
{
    uint16_t sum = (uint16_t)i_a + (uint16_t)i_b + cy;
	cc.z = ((uint8_t)(i_a + i_b + cy) == 0);
	cc.s = ((sum & 0x80) == 0x80);
	cc.p = (Parity((uint8_t)(i_a + i_b + cy)));
	uint16_t carry = sum ^ i_a ^ i_b;
	/* cc.cy = (carry & (1 << 8)); */
	cc.cy = sum > 0xff;
    /* cc.ac = (carry & (1 << 4)); */
}

void CPU_8080::subtraction_flags(uint8_t i_a, uint8_t i_b, uint8_t cy)
{
    uint16_t dif = (uint16_t)a - (uint16_t)b - cy;
	cc.z = (dif == 0);
	cc.s = ((dif & 0x80) == 0x80);
	cc.p = (Parity((uint8_t)(a - b - cy)));
	cc.cy = (a < (b + cy)) ? 1 : 0;
	cc.ac = ~(a ^ b ^ dif) & 0x10;
}
