#include "i8080.h"
#include "memory.h"

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

    instructions_executed = 0;
}

void CPU_8080::UnimplementedInstruction(uint8_t opcode)
{
    halted = true;
}

void CPU_8080::EmulateCycles(uint32_t num_cycles)
{
    cycles = 0;
    while(cycles < num_cycles) {
        RegularInstruction();
    }
}

void CPU_8080::RegularInstruction()
{
    // Check PC boundary
    if (pc > 0x3fff) {
        halted = true;
        return;
    }
    // Show the disassembled instruction before executing
    // Fetch opcode
    uint8_t opcode = MemoryRead(pc);
    ExecuteInstruction(opcode);
}

void CPU_8080::Interrupt(uint8_t opcode)
{
    // If interrupts are enabled, service the request
    if (int_enable) {
        int_enable = false;
        ExecuteInstruction(opcode);
    }
}

void CPU_8080::ExecuteInstruction(uint8_t opcode)
{
#define BYTE1 MemoryRead(pc+1)
#define BYTE2 MemoryRead(pc+2)
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
        /*   0x08: - */
        case 0x09:      DAD (b, c);                               break;
        case 0x0a:     LDAX (b, c);                               break;
        case 0x0b:    DCX_B ();                                   break;
        case 0x0c:    INR_r (c);                                  break;
        case 0x0d:    DCR_r (c);                                  break;
        case 0x0e:      MVI (c, BYTE1);                           break;
        case 0x0f:      RRC ();                                   break;
        /*   0x10: - */
        case 0x11:      LXI (d, e, BYTE2, BYTE1);                 break;
        case 0x12:   STAX_D ();                                   break;
        case 0x13:      INX (d, e);                               break;
        case 0x14:    INR_r (d);                                  break;
        case 0x15:    DCR_r (d);                                  break;
        case 0x16:      MVI (d, BYTE1);                           break;
        case 0x17:      RAL ();                                   break;
        /*   0x18: - */
        case 0x19:      DAD (d, e);                               break;
        case 0x1a:     LDAX (d, e);                               break;
        case 0x1b:    DCX_D ();                                   break;
        case 0x1c:    INR_r (e);                                  break;
        case 0x1d:    DCR_r (e);                                  break;
        case 0x1e:      MVI (e, BYTE1);                           break;
        case 0x1f:      RAR ();                                   break;
        /*   0x20: - */
        case 0x21:      LXI (h, l, BYTE2, BYTE1);                 break;
        case 0x22:     SHLD (BYTE2, BYTE1);                       break;
        case 0x23:      INX (h, l);                               break;
        case 0x24:    INR_r (h);                                  break;
        case 0x25:    DCR_r (h);                                  break;
        case 0x26:      MVI (h, BYTE1);                           break;
        case 0x27: pc+=1; break; //TODO SKIP!
        /*   0x28: - */
        case 0x29:      DAD (h, l);                               break;
        case 0x2a:     LHLD (BYTE2, BYTE1);                       break;
        case 0x2b:    DCX_H ();                                   break;
        case 0x2c:    INR_r (l);                                  break;
        case 0x2d:    DCR_r (l);                                  break;
        case 0x2e:      MVI (l, BYTE1);                           break;
        case 0x2f:      CMA ();                                   break;
        /*   0x30: - */
        case 0x31:   LXI_SP (BYTE2, BYTE1);                       break;
        case 0x32:      STA (BYTE2, BYTE1);                       break;
        case 0x33:   INX_SP ();                                   break;
        case 0x34:    INR_M ();                                   break;
        case 0x35:    DCR_M ();                                   break;
        case 0x36:    MVI_M (BYTE1);                              break;
        case 0x37:      STC ();                                   break;
        /*   0x38: - */
        case 0x39:   DAD_SP ();                                   break;
        case 0x3a:      LDA (BYTE2, BYTE1);                       break;
        case 0x3b:      DCX_SP ();                                break;
        case 0x3c:    INR_r (a);                                  break;
        case 0x3d:    DCR_r (a);                                  break;
        case 0x3e:      MVI (a, BYTE1);                           break;
        case 0x3f:      CMC ();                                   break;
        case 0x40:  MOV_r_r (b, b);                               break;
        case 0x41:  MOV_r_r (b, c);                               break;
        case 0x42:  MOV_r_r (b, d);                               break;
        case 0x43:  MOV_r_r (b, e);                               break;
        case 0x44:  MOV_r_r (b, h);                               break;
        case 0x45:  MOV_r_r (b, l);                               break;
        case 0x46:  MOV_r_m (b);                                  break;
        case 0x47:  MOV_r_r (b, a);                               break;
        case 0x48:  MOV_r_r (c, b);                               break;
        case 0x49:  MOV_r_r (c, c);                               break;
        case 0x4a:  MOV_r_r (c, d);                               break;
        case 0x4b:  MOV_r_r (c, e);                               break;
        case 0x4c:  MOV_r_r (c, h);                               break;
        case 0x4d:  MOV_r_r (c, l);                               break;
        case 0x4e:  MOV_r_m (c);                                  break;
        case 0x4f:  MOV_r_r (c, a);                               break;
        case 0x50:  MOV_r_r (d, b);                               break;
        case 0x51:  MOV_r_r (d, c);                               break;
        case 0x52:  MOV_r_r (d, d);                               break;
        case 0x53:  MOV_r_r (d, e);                               break;
        case 0x54:  MOV_r_r (d, h);                               break;
        case 0x55:  MOV_r_r (d, l);                               break;
        case 0x56:  MOV_r_m (d);                                  break;
        case 0x57:  MOV_r_r (d, a);                               break;
        case 0x58:  MOV_r_r (e, b);                               break;
        case 0x59:  MOV_r_r (e, c);                               break;
        case 0x5a:  MOV_r_r (e, d);                               break;
        case 0x5b:  MOV_r_r (e, e);                               break;
        case 0x5c:  MOV_r_r (e, h);                               break;
        case 0x5d:  MOV_r_r (e, l);                               break;
        case 0x5e:  MOV_r_m (e);                                  break;
        case 0x5f:  MOV_r_r (e, a);                               break;
        case 0x60:  MOV_r_r (h, b);                               break;
        case 0x61:  MOV_r_r (h, c);                               break;
        case 0x62:  MOV_r_r (h, d);                               break;
        case 0x63:  MOV_r_r (h, e);                               break;
        case 0x64:  MOV_r_r (h, h);                               break;
        case 0x65:  MOV_r_r (h, l);                               break;
        case 0x66:  MOV_r_m (h);                                  break;
        case 0x67:  MOV_r_r (h, a);                               break;
        case 0x68:  MOV_r_r (l, b);                               break;
        case 0x69:  MOV_r_r (l, c);                               break;
        case 0x6a:  MOV_r_r (l, d);                               break;
        case 0x6b:  MOV_r_r (l, e);                               break;
        case 0x6c:  MOV_r_r (l, h);                               break;
        case 0x6d:  MOV_r_r (l, l);                               break;
        case 0x6e:  MOV_r_m (l);                                  break;
        case 0x6f:  MOV_r_r (l, a);                               break;
        case 0x70:  MOV_m_r (b);                                  break;
        case 0x71:  MOV_m_r (c);                                  break;
        case 0x72:  MOV_m_r (d);                                  break;
        case 0x73:  MOV_m_r (e);                                  break;
        case 0x74:  MOV_m_r (h);                                  break;
        case 0x75:  MOV_m_r (l);                                  break;
        /* case 0x76:  HLT ();                                  break; */
        case 0x77:  MOV_m_r (a);                                  break;
        case 0x78:  MOV_r_r (a, b);                               break;
        case 0x79:  MOV_r_r (a, c);                               break;
        case 0x7a:  MOV_r_r (a, d);                               break;
        case 0x7b:  MOV_r_r (a, e);                               break;
        case 0x7c:  MOV_r_r (a, h);                               break;
        case 0x7d:  MOV_r_r (a, l);                               break;
        case 0x7e:  MOV_r_m (a);                                  break;
        case 0x7f:  MOV_r_r (a, a);                               break;
        case 0x80:    ADD_r (b);                                  break;
        case 0x81:    ADD_r (c);                                  break;
        case 0x82:    ADD_r (d);                                  break;
        case 0x83:    ADD_r (e);                                  break;
        case 0x84:    ADD_r (h);                                  break;
        case 0x85:    ADD_r (l);                                  break;
        case 0x86:    ADD_M ();                                   break;
        case 0x87:    ADD_r (a);                                  break;
        case 0x88:    ADC_r (b);                                  break;
        case 0x89:    ADC_r (c);                                  break;
        case 0x8a:    ADC_r (d);                                  break;
        case 0x8b:    ADC_r (e);                                  break;
        case 0x8c:    ADC_r (h);                                  break;
        case 0x8d:    ADC_r (l);                                  break;
        case 0x8e:    ADC_M ();                                   break;
        case 0x8f:    ADC_r (a);                                  break;
        case 0x90:    SUB_r (b);                                  break;
        case 0x91:    SUB_r (c);                                  break;
        case 0x92:    SUB_r (d);                                  break;
        case 0x93:    SUB_r (e);                                  break;
        case 0x94:    SUB_r (h);                                  break;
        case 0x95:    SUB_r (l);                                  break;
        case 0x96:    SUB_M ();                                   break;
        case 0x97:    SUB_r (a);                                  break;
        case 0x98:    SBB_r (b);                                  break;
        case 0x99:    SBB_r (c);                                  break;
        case 0x9a:    SBB_r (d);                                  break;
        case 0x9b:    SBB_r (e);                                  break;
        case 0x9c:    SBB_r (h);                                  break;
        case 0x9d:    SBB_r (l);                                  break;
        case 0x9e:    SBB_M ();                                   break;
        case 0x9f:    SBB_r (a);                                  break;
        case 0xa0:    ANA_r (b);                                  break;
        case 0xa1:    ANA_r (c);                                  break;
        case 0xa2:    ANA_r (d);                                  break;
        case 0xa3:    ANA_r (e);                                  break;
        case 0xa4:    ANA_r (h);                                  break;
        case 0xa5:    ANA_r (l);                                  break;
        case 0xa6:    ANA_M ();                                   break;
        case 0xa7:    ANA_r (a);                                  break;
        case 0xa8:    XRA_r (b);                                  break;
        case 0xa9:    XRA_r (c);                                  break;
        case 0xaa:    XRA_r (d);                                  break;
        case 0xab:    XRA_r (e);                                  break;
        case 0xac:    XRA_r (h);                                  break;
        case 0xad:    XRA_r (l);                                  break;
        case 0xae:    XRA_M ();                                   break;
        case 0xaf:    XRA_r (a);                                  break;
        case 0xb0:    ORA_r (b);                                  break;
        case 0xb1:    ORA_r (c);                                  break;
        case 0xb2:    ORA_r (d);                                  break;
        case 0xb3:    ORA_r (e);                                  break;
        case 0xb4:    ORA_r (h);                                  break;
        case 0xb5:    ORA_r (l);                                  break;
        case 0xb6:    ORA_M ();                                   break;
        case 0xb7:    ORA_r (a);                                  break;
        case 0xb8:    CMP_r (b);                                  break;
        case 0xb9:    CMP_r (c);                                  break;
        case 0xba:    CMP_r (d);                                  break;
        case 0xbb:    CMP_r (e);                                  break;
        case 0xbc:    CMP_r (h);                                  break;
        case 0xbd:    CMP_r (l);                                  break;
        case 0xbe:    CMP_M ();                                   break;
        case 0xbf:    CMP_r (a);                                  break;
        case 0xc0:   R_cond (!cc.z);                              break;
        case 0xc1:   POP_rp (b, c);                               break;
        case 0xc2:   J_Cond (BYTE2, BYTE1, !cc.z);                break;
        case 0xc3:   J_Cond (BYTE2, BYTE1, true);                 break;
        case 0xc4:   C_Cond (BYTE2, BYTE1, (!cc.z));              break;
        case 0xc5:  PUSH_rp (b, c);                               break;
        case 0xc6:      ADI (BYTE1);                              break;
        case 0xc7:      RST (0x0);                                break;
        case 0xc8:   R_cond (cc.z);                               break;
        case 0xc9:      RET ();                                   break;
        case 0xca:   J_Cond (BYTE2, BYTE1, cc.z);                 break;
        /*   0xcb: - */
        case 0xcc:   C_Cond (BYTE2, BYTE1, cc.z);                 break;
        case 0xcd:     CALL (BYTE2, BYTE1);                       break;
        case 0xce:      ACI (BYTE1);                              break;
        case 0xcf:      RST (0x1);                                break;
        case 0xd0:   R_cond (!cc.cy);                             break;
        case 0xd1:   POP_rp (d, e);                               break;
        case 0xd2:   J_Cond (BYTE2, BYTE1, !cc.cy);               break;
        case 0xd3:      OUT (BYTE1);                              break;
        case 0xd4:   C_Cond (BYTE2, BYTE1, !cc.cy);               break;
        case 0xd5:  PUSH_rp (d, e);                               break;
        case 0xd6:      SUI (BYTE1);                              break;
        case 0xd7:      RST (0x2);                                break;
        case 0xd8:   R_cond (cc.cy);                              break;
        /*   0xd9: - */
        case 0xda:   J_Cond (BYTE2, BYTE1, cc.cy);                break;
        case 0xdb:       IN (BYTE1);                              break;
        case 0xdc:   C_Cond (BYTE2, BYTE1, cc.cy);                break;
        /*   0xdd: - */
        case 0xde:      SBI (BYTE1);                              break;
        case 0xdf:      RST (0x3);                                break;
        case 0xe0:   R_cond (!cc.p);                               break;
        case 0xe1:   POP_rp (h, l);                               break;
        case 0xe2:   J_Cond (BYTE2, BYTE1, !cc.p);                break;
        case 0xe3:     XTHL ();                                   break;
        case 0xe4:   C_Cond (BYTE2, BYTE1, !cc.p);                break;
        case 0xe5:  PUSH_rp (h, l);                               break;
        case 0xe6:      ANI (BYTE1);                              break;
        case 0xe7:      RST (0x4);                                break;
        case 0xe8:   R_cond (cc.p);                               break;
        case 0xe9:     PCHL ();                                   break;
        case 0xea:   J_Cond (BYTE2, BYTE1, cc.p);                 break;
        case 0xeb:     XCHG ();                                   break;
        case 0xec:   C_Cond (BYTE2, BYTE1, cc.p);                 break;
        /*   0xed: - */
        case 0xee:      XRI (BYTE1);                              break;
        case 0xef:      RST (0x5);                                break;
        case 0xf0:   R_cond (!cc.s);                              break;
        case 0xf1:  POP_PSW ();                                   break;
        case 0xf2:   J_Cond (BYTE2, BYTE1, !cc.s);                break;
        case 0xf3:       DI ();                                   break;
        case 0xf4:   C_Cond (BYTE2, BYTE1, !cc.s);                break;
        case 0xf5: PUSH_PSW ();                                   break;
        case 0xf6:      ORI (BYTE1);                              break;
        case 0xf7:      RST (0x6);                                break;
        case 0xf8:   R_cond (cc.s);                               break;
        case 0xf9:     SPHL ();                                   break;
        case 0xfa:   J_Cond (BYTE2, BYTE1, cc.s);                 break;
        case 0xfb:       EI ();                                   break;
        case 0xfc:   C_Cond (BYTE2, BYTE1, cc.s);                 break;
        /*   0xfd: - */
        case 0xfe:      CPI (BYTE1);                              break;
        case 0xff:      RST (0x7);                                break;
        default  : UnimplementedInstruction(opcode);              break;
    }

    instructions_executed++;
}

bool CPU_8080::Running()
{
    return !halted;
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
}

uint8_t CPU_8080::MemoryRead(uint16_t address)
{
    return m_memory->Read(address);
}

void CPU_8080::MemoryWrite(uint16_t address, uint8_t data)
{
    m_memory->Write(address, data);
}

void CPU_8080::Connect(Io_devices* l_devices, Memory* i_memory)
{
    devices = l_devices;
    m_memory = i_memory;
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
	cc.p = (Parity(sum & 0xff));
	uint16_t carry = sum ^ i_a ^ i_b;
	/* cc.cy = (carry & (1 << 8)); */
	cc.cy = sum > 0xff;
    /* cc.ac = (carry & (1 << 4)); */
}

void CPU_8080::subtraction_flags(uint8_t i_a, uint8_t i_b, uint8_t i_cy)
{
    uint16_t dif = (uint16_t)i_a - (uint16_t)i_b - i_cy;
	cc.z = (dif == 0);
	cc.s = ((dif & 0x80) == 0x80);
	cc.p = (Parity((uint8_t)(i_a - i_b - i_cy)));
	cc.cy = (i_a < (i_b + i_cy)) ? 1 : 0;
	cc.ac = ~(i_a ^ i_b ^ dif) & 0x10;
}
