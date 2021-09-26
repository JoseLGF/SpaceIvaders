/*
 * File: i8080.h
 * Description: Header definitions for the i8080 cpu class.
 * */

#ifndef I8080_H
#define I8080_H

#include <cstdint>

// Forward declarations
class Io_devices;
class Memory;

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
    bool Running();
    void Interrupt(uint8_t opcode);
    void Connect(Io_devices* devices, Memory* memory);

    /* Memory Read and Write interface */
    uint8_t MemoryRead(uint16_t address);
    void MemoryWrite(uint16_t address, uint8_t data);
    /* Internal functions */
    void RegularInstruction();


private:
    // External devices class connected to the cpu
    Io_devices* devices;
    Memory *    m_memory;

// CPU state
public:
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
    void J_Cond(uint8_t hi, uint8_t lo, bool cond); // Tested
    void PCHL();                                    //
    // Other group
    void NOP();                                     // Skip
    // Stack group
    void LXI_SP(uint8_t hi, uint8_t lo);            // Tested
    void PUSH_rp(uint8_t& r1, uint8_t& r2);         // Tested
    void POP_rp(uint8_t& r1, uint8_t& r2);          // Tested
    void PUSH_PSW();                                // Tested
    void POP_PSW();                                 // Tested
    void XTHL();                                    // Skip
    void SPHL();
    // Move group
    void MVI(uint8_t& reg, uint8_t data);           // Tested
    void MOV_r_r(uint8_t& r1, uint8_t& r2);         // Tested
    void MOV_r_m(uint8_t& r);                       // Tested
    void MOV_m_r(uint8_t& r);                       // Tested
    void MVI_M(uint8_t data);                       // Tested
    void LXI(uint8_t& r1, uint8_t& r2, uint8_t d1, uint8_t d2); // Tested
    void LDAX(uint8_t& r1, uint8_t& r2);            // Tested
    void STA(uint8_t byte_h, uint8_t byte_l);       // Tested
    void LDA(uint8_t byte_h, uint8_t byte_l);       // Tested
    void XCHG();                                    // Tested
    void STAX_B();                                  // Tested
    void STAX_D();                                  // Tested
    void LHLD(uint8_t byte_h, uint8_t byte_l);      // Tested
    void SHLD(uint8_t byte_h, uint8_t byte_l);      // Tested
    // Call group
    void CALL(uint8_t hi, uint8_t lo);              // Tested
    void CALL(uint16_t address);
    void C_Cond(uint8_t hi, uint8_t lo, bool cond); // Tested
    void RET();                                     // Tested
    void RST(uint8_t exp);                          // Pending
    void R_cond(bool cond);                         // Tested
    // Increment and decrement group
    void INR_r(uint8_t& r);                         // Tested
    void INR_M();                                   // Tested
    void DCR_r(uint8_t& r);                         // Tested
    void INX(uint8_t& r1, uint8_t& r2);             // Tested
    void INX_SP();
    void DCX_H();                                   // Tested
    void DCX_B();                                   // Tested
    void DCX_D();                                   // Tested
    void DCX_SP();                                   // Tested
    void DCR_M();                                   // Tested
    // Add and Subtract groups
    void ADD_M();                                   // Tested
    void ADC_M();                                   // Tested
    void ADD_r(uint8_t& r);                         // Tested
    void ADC_r(uint8_t& r);                         // Tested
    void DAD(uint8_t& r1, uint8_t& r2);             // Tested
    void DAD_SP();
    void ADI(uint8_t data);                         // Tested
    void ACI(uint8_t data);                         // Tested
    void SUI(uint8_t data);                         // Tested
    void SUB_r(uint8_t& r);                         // Tested
    void SBB_r(uint8_t& r);                         // Tested
    void SUB_M();
    void SBB_M();
    void SBI(uint8_t data);                         // Tested
    // Logical and rotate groups
    void ANA_r(uint8_t& r);                         // Tested
    void ANA_M();                                   // Tested
    void XRA_r(uint8_t& r);                         // Tested
    void XRA_M();                         // Tested
    void ORA_r(uint8_t& r);                         // Tested
    void ORA_M();                                   // Tested
    void CMP_r(uint8_t& r);                         // Tested
    void CMP_M();                                   // Skip
    void ORI(uint8_t data);                         // Tested
    void ANI(uint8_t data);                         // Tested
    void XRI(uint8_t data);                         // Tested
    void CPI(uint8_t data);                         // Tested
    void RRC();                                     // Tested
    void RLC();                                     // Tested
    void RAR();                                     // Tested
    void RAL();                                     // Tested
    // IO and special groups
    void OUT(uint8_t byte);                         // Pending
    void IN(uint8_t device);                        // Pending
    void EI();
    void DI();
    void STC();                                     // Tested
    void CMA();                                     //
    void CMC();                                     //

    // Internal
    bool Parity(uint8_t byte);
    void logical_flags(uint8_t result);
    void addition_flags(uint8_t a, uint8_t b, uint8_t cy);
    void subtraction_flags(uint8_t a, uint8_t b, uint8_t cy);
    uint8_t read_rp(uint8_t r1, uint8_t r2);
    uint8_t ReadFromRegPair(uint8_t& hi, uint8_t& lo);
    void ZSPFlags(uint8_t result);
};


#endif
