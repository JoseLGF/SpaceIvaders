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

    uint16_t instructions_executed;

    // For debugging
    bool halted = false;
    void UnimplementedInstruction(uint8_t opcode);

    // Implementations for the opcodes
    // Branch group
    void JMP(uint8_t hi, uint8_t lo);
    void JNZ(uint8_t hi, uint8_t lo);
    // Other group
    void NOP();
    // Stack group
    void LXI_SP(uint8_t hi, uint8_t lo);
    // Move group
    void MVI_B(uint8_t data);
    void MOV_M_A();
    void LXI_B(uint8_t byte_b, uint8_t byte_c);
    void LXI_D(uint8_t byte_d, uint8_t byte_e);
    void LXI_H(uint8_t byte_h, uint8_t byte_l);
    void LDAX_D();
    // Call group
    void CALL(uint8_t hi, uint8_t lo);
    void RET();
    // Increment and decrement group
    void INX_H();
    void INX_D();
    void DCR_B();
    // Add and Subtract groups
    void DAD_D();

    // Internal
    bool Parity(uint8_t byte);


};
