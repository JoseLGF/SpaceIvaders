/*
 * File: i8080.h
 * Description: Header definitions for the i8080 cpu class.
 * */

 #include <string>

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
	void EmulateCycle();
    void LoadRom();
    void PrintState();
    bool Running();
    void DumpMemory();

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

    uint8_t ReadMemoryAt(uint16_t address);
    void WriteMemoryAt(uint16_t address, uint8_t data);

// CPU state
private:
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

    // For debugging
    bool halted = false;
    void UnimplementedInstruction(uint8_t opcode);

    // Implementations for the opcodes
    // Branch group
    void JMP(uint8_t hi, uint8_t lo);               // Tested
    void JNZ(uint8_t hi, uint8_t lo);               // Tested
    // Other group
    void NOP();                                     // Skip
    // Stack group
    void LXI_SP(uint8_t hi, uint8_t lo);            // Skip
    // Move group
    void MVI_B(uint8_t data);                       // Skip
    void MVI_A(uint8_t data);                       // Skip
    void MOV_M_A();                                 // Tested
    void LXI_B(uint8_t byte_b, uint8_t byte_c);     // Skip
    void LXI_D(uint8_t byte_d, uint8_t byte_e);     // Skip
    void LXI_H(uint8_t byte_h, uint8_t byte_l);     // Skip
    void LDAX_D();                                  // Skip
    void STA(uint8_t byte_h, uint8_t byte_l);       // Tested
    void LDA(uint8_t byte_h, uint8_t byte_l);       // Tested
    // Call group
    void CALL(uint8_t hi, uint8_t lo);              // Tested
    void RET();                                     // Tested
    // Increment and decrement group
    void INX_H();
    void INX_D();
    void DCR_B();
    // Add and Subtract groups
    void DAD_D();
    // Logical group
    void XRA_A();
    void ANA_A();
    // IO and special groups
    void OUT(uint8_t byte);
    void EI();

    // Internal
    bool Parity(uint8_t byte);


};
