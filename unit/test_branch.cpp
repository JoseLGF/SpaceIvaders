/*
 * Test file for branch group instructions
 *
 */

#include <string>
#include <gtest/gtest.h>

#include "../include/i8080.h"

using namespace std;

TEST(BranchGroup, JMP_VerifyPCUpdatedAfterJMP) {
    CPU_8080 cpu;
    cpu.Initialize();
    cpu.WriteMemoryAt(0x0000, 0xc3); // JMP instruction
    cpu.WriteMemoryAt(0x0001, 0xd4); // Lo byte of jmp adr
    cpu.WriteMemoryAt(0x0002, 0x18); // Hi byte of jmp adr

    cpu.RegularInstruction();

    ASSERT_EQ(0x18d4, cpu.Get_pc());
}

TEST(BranchGroup, JNZ_WhenZIsClearedThenJmpToSpecifiedAddress) {
    CPU_8080 cpu;
    cpu.Initialize();
    cpu.Set_z(false);

    cpu.WriteMemoryAt(0x0000, 0xc2); // JNZ instruction
    cpu.WriteMemoryAt(0x0001, 0xd4); // Lo byte of jmp adr
    cpu.WriteMemoryAt(0x0002, 0x18); // Hi byte of jmp adr

    cpu.RegularInstruction();

    ASSERT_EQ(0x18d4, cpu.Get_pc());
}

TEST(BranchGroup, JNZ_WhenZIsSetThenDontJumpToAddress) {
    CPU_8080 cpu;
    cpu.Initialize();
    cpu.Set_z(true);

    // PC = 0x0000
    cpu.WriteMemoryAt(0x0000, 0xc2); // JNZ instruction
    cpu.WriteMemoryAt(0x0001, 0xd4); // Lo byte of jmp adr
    cpu.WriteMemoryAt(0x0002, 0x18); // Hi byte of jmp adr

    cpu.RegularInstruction();

    ASSERT_EQ(0x0003, cpu.Get_pc());
}
