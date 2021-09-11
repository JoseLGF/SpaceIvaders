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
    uint8_t     ac  : 1;
    uint8_t     pad : 3;
} ConditionCodes;

class CPU_8080 {
public:

    /* Public interface */
	void Initialize();
	//void EmulateCycle();
    void LoadRom();
    void PrintState();

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
    uint8_t     memory[0x2000];
    // Processor flags
    struct ConditionCodes cc;
    // interrupt enable
    uint8_t     int_enable;
};
