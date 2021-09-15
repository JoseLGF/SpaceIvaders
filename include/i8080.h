/*
 * File: i8080.h
 * Description: Header definitions for the i8080 cpu class.
 * */

#include <string>
#include "io_devices.h"

// Processor flags
typedef struct ConditionCodes {
    uint8_t     z   : 1;
    uint8_t     s   : 1;
    uint8_t     p   : 1;
    uint8_t     cy  : 1;
    uint8_t     ac  : 1; //  Unused in Space Invaders
    uint8_t     pad : 3;
} ConditionCodes;

class CPU_8080 {
public:
    /* Public interface */
	void Initialize();
	void ExecuteInstruction(uint8_t opcode);
	void EmulateCycles(uint32_t num_cycles);
    void LoadRom(std::string filename);
    void PrintState();
    bool Running();
    void DumpMemory();
    void Interrupt(uint8_t opcode);
    void Connect_io_dev(Io_devices* devices);

public:
    /* Memory Read and Write interface */
    uint8_t MemoryRead(uint16_t address);
    void MemoryWrite(uint16_t address, uint8_t data);

public:
    /* Internal functions */
    void RegularInstruction();

public:
    /* Functions for unit testing */
    bool Get_z();
    bool Get_s();
    bool Get_p();
    bool Get_cy();
    bool Get_ac();
    void Set_z(bool bit);
    void Set_s(bool bit);
    void Set_p(bool bit);
    void Set_cy(bool bit);
    void Set_ac(bool bit);

    uint8_t Get_a();
    uint8_t Get_b();
    uint8_t Get_c();
    uint8_t Get_d();
    uint8_t Get_e();
    uint8_t Get_h();
    uint8_t Get_l();
    void Set_a(uint8_t data);
    void Set_b(uint8_t data);
    void Set_c(uint8_t data);
    void Set_d(uint8_t data);
    void Set_e(uint8_t data);
    void Set_h(uint8_t data);
    void Set_l(uint8_t data);

    uint16_t Get_sp();
    void Set_sp(uint16_t address);
    uint16_t Get_pc();
    void Set_pc(uint16_t address);

    uint8_t ReadMemoryAt(uint16_t address);
    void WriteMemoryAt(uint16_t address, uint8_t data);

// CPU state
private:
    // External devices class connected to the cpu
    Io_devices* devices;

    // Accumulator register a
    uint8_t     a;
    // General purpose registers b,c,d,e
    uint8_t     b;
    uint8_t     c;
    uint8_t     d;
    uint8_t     e;
    // Memory pair h, l registers
    uint8_t     h;
    uint8_t     l;
    // Stack pointer
    uint16_t    sp;
    // Program counter
    uint16_t    pc;
    // Memory
    // 0x0000 - 0x1fff - Game  ROM
    // 0x2000 - 0x23ff - Work  RAM
    // 0x2400 - 0x3fff - Video RAM
    // 0x4000 - ------ - RAM mirror
    uint8_t     memory[0x3fff];
    // Processor flags
    struct ConditionCodes cc;
    // interrupt enable
    uint8_t     int_enable;

    long int instructions_executed;
    long int cycles;

    // For debugging
    bool halted = false;
    void UnimplementedInstruction(uint8_t opcode);

    // Implementations for the opcodes
    // Branch group
    void JMP(uint8_t hi, uint8_t lo);               // Tested
    void JM(uint8_t hi, uint8_t lo);                // Tested
    void JNZ(uint8_t hi, uint8_t lo);               // Tested
    void JNC(uint8_t hi, uint8_t lo);               // Tested
    void JC(uint8_t hi, uint8_t lo);                // Tested
    void JZ(uint8_t hi, uint8_t lo);                // Tested
    // Other group
    void NOP();                                     // Skip
    // Stack group
    void LXI_SP(uint8_t hi, uint8_t lo);            // Tested
    void PUSH_B();                                  // Tested
    void PUSH_D();                                  // Tested
    void PUSH_H();                                  // Tested
    void PUSH_PSW();                                // Tested
    void POP_H();                                   // Tested
    void POP_B();                                   // Tested
    void POP_D();                                   // Tested
    void POP_PSW();                                 // Tested
    // Move group
    void MVI(uint8_t& reg, uint8_t data);           // Tested
    void MOV_r_r(uint8_t& r1, uint8_t& r2);         // Tested
    void MOV_r_m(uint8_t& r);                       // Tested
    void MOV_m_r(uint8_t& r);                       // Tested
    void MVI_M(uint8_t data);                       // Tested
    void LXI_B(uint8_t byte_b, uint8_t byte_c);     // Tested
    void LXI_D(uint8_t byte_d, uint8_t byte_e);     // Tested
    void LXI_H(uint8_t byte_h, uint8_t byte_l);     // Tested
    void LDAX_D();                                  // Tested
    void LDAX_B();                                  // Tested
    void STA(uint8_t byte_h, uint8_t byte_l);       // Tested
    void LDA(uint8_t byte_h, uint8_t byte_l);       // Tested
    void XCHG();                                    // Tested
    void STAX_B();                                  // Tested
    void LHLD(uint8_t byte_h, uint8_t byte_l);      // Tested
    void SHLD(uint8_t byte_h, uint8_t byte_l);      // Tested
    // Call group
    void CALL(uint8_t hi, uint8_t lo);              // Tested
    void CNZ(uint8_t hi, uint8_t lo);               // Skip
    void CZ(uint8_t hi, uint8_t lo);                // Skip
    void RET();                                     // Tested
    void RST(uint8_t exp);                          // Pending
    void RZ();                                      // Tested
    void RNZ();                                     // Tested
    void RC();                                      // Tested
    void RNC();                                     // Tested
    // Increment and decrement group
    void INR_r(uint8_t& r);                         // Tested
    void DCR_r(uint8_t& r);                         // Tested
    void INX_H();                                   // Tested
    void DCX_H();                                   // Tested
    void INX_D();                                   // Tested
    void INX_B();                                   // Tested
    void DCR_M();                                   // Tested
    // Add and Subtract groups
    void ADD_M();                                   // Tested
    void DAD_B();                                   // Tested
    void DAD_D();                                   // Tested
    void DAD_H();                                   // Tested
    void ADI(uint8_t data);                         // Tested
    void SUI(uint8_t data);                         // Tested
    void SBI(uint8_t data);                         // Tested
    // Logical and rotate groups
    void ANA_r(uint8_t& r);                         // Tested
    void XRA_r(uint8_t& r);                         // Tested
    void ORA_r(uint8_t& r);                         // Tested
    void ORA_M();                                   // Tested
    void ORA_B();                                   // Tested
    void ORA_H();                                   // Tested
    void ORI(uint8_t data);                         // Tested
    void ANI(uint8_t data);                         // Tested
    void CPI(uint8_t data);                         // Tested
    void RRC();                                     // Tested
    void RLC();                                     // Tested
    void RAR();                                     // Tested
    // IO and special groups
    void OUT(uint8_t byte);                         // Pending
    void IN(uint8_t device);                        // Pending
    void EI();
    void STC();                                     // Tested

    // Internal
    bool Parity(uint8_t byte);
};
