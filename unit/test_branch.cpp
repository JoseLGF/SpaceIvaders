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

TEST(BranchGroup, JC_WhenCyIsSetThenJmpToSpecifiedAddress) {
    CPU_8080 cpu;
    cpu.Initialize();
    cpu.Set_cy(true);

    cpu.WriteMemoryAt(0x0000, 0xda); // JC instruction
    cpu.WriteMemoryAt(0x0001, 0xd4); // Lo byte of jmp adr
    cpu.WriteMemoryAt(0x0002, 0x18); // Hi byte of jmp adr

    cpu.RegularInstruction();

    ASSERT_EQ(0x18d4, cpu.Get_pc());
}

TEST(BranchGroup, JC_WhenCyIsClearedThenDontJmpToSpecifiedAddress) {
    CPU_8080 cpu;
    cpu.Initialize();
    cpu.Set_cy(false);

    cpu.WriteMemoryAt(0x0000, 0xda); // JC instruction
    cpu.WriteMemoryAt(0x0001, 0xd4); // Lo byte of jmp adr
    cpu.WriteMemoryAt(0x0002, 0x18); // Hi byte of jmp adr

    cpu.RegularInstruction();

    ASSERT_EQ(0x0003, cpu.Get_pc());
}

TEST(BranchGroup, JZ_WhenZIsSetThenJmpToSpecifiedAddress) {
    CPU_8080 cpu;
    cpu.Initialize();
    cpu.Set_z(true);

    cpu.WriteMemoryAt(0x0000, 0xca); // JZ instruction
    cpu.WriteMemoryAt(0x0001, 0xd4); // Lo byte of jmp adr
    cpu.WriteMemoryAt(0x0002, 0x18); // Hi byte of jmp adr

    cpu.RegularInstruction();

    ASSERT_EQ(0x18d4, cpu.Get_pc());
}

TEST(BranchGroup, JZ_WhenZIsClearedThenDontJmpToSpecifiedAddress) {
    CPU_8080 cpu;
    cpu.Initialize();
    cpu.Set_z(false);

    cpu.WriteMemoryAt(0x0000, 0xca); // JZ instruction
    cpu.WriteMemoryAt(0x0001, 0xd4); // Lo byte of jmp adr
    cpu.WriteMemoryAt(0x0002, 0x18); // Hi byte of jmp adr

    cpu.RegularInstruction();

    ASSERT_EQ(0x0003, cpu.Get_pc());
}

TEST(BranchGroup, JNC_WhenCyIsClearedThenJmpToSpecifiedAddress) {
    CPU_8080 cpu;
    cpu.Initialize();
    cpu.Set_cy(false);

    cpu.WriteMemoryAt(0x0000, 0xd2); // JNC instruction
    cpu.WriteMemoryAt(0x0001, 0xd4); // Lo byte of jmp adr
    cpu.WriteMemoryAt(0x0002, 0x18); // Hi byte of jmp adr

    cpu.RegularInstruction();

    ASSERT_EQ(0x18d4, cpu.Get_pc());
}

TEST(BranchGroup, JNC_WhenCyIsSetThenDontJmpToSpecifiedAddress) {
    CPU_8080 cpu;
    cpu.Initialize();
    cpu.Set_cy(true);

    cpu.WriteMemoryAt(0x0000, 0xd2); // JNC instruction
    cpu.WriteMemoryAt(0x0001, 0xd4); // Lo byte of jmp adr
    cpu.WriteMemoryAt(0x0002, 0x18); // Hi byte of jmp adr

    cpu.RegularInstruction();

    ASSERT_EQ(0x0003, cpu.Get_pc());
}

TEST(BranchGroup, JM_WhenSIsSetThenJmpToSpecifiedAddress) {
    CPU_8080 cpu;
    cpu.Initialize();
    cpu.Set_s(true);

    cpu.WriteMemoryAt(0x0000, 0xfa); // JM instruction
    cpu.WriteMemoryAt(0x0001, 0xd4); // Lo byte of jmp adr
    cpu.WriteMemoryAt(0x0002, 0x18); // Hi byte of jmp adr

    cpu.RegularInstruction();

    ASSERT_EQ(0x18d4, cpu.Get_pc());
}

TEST(BranchGroup, JM_WhenSIsClearedThenDontJmpToSpecifiedAddress) {
    CPU_8080 cpu;
    cpu.Initialize();
    cpu.Set_s(false);

    cpu.WriteMemoryAt(0x0000, 0xfa); // JM instruction
    cpu.WriteMemoryAt(0x0001, 0xd4); // Lo byte of jmp adr
    cpu.WriteMemoryAt(0x0002, 0x18); // Hi byte of jmp adr

    cpu.RegularInstruction();

    ASSERT_EQ(0x0003, cpu.Get_pc());
}

TEST(BranchGroup, PCHL) {
    CPU_8080 cpu;
    cpu.Initialize();
    cpu.Set_h(0x23);
    cpu.Set_l(0x45);

    cpu.WriteMemoryAt(0x0000, 0xe9); // PCHL

    cpu.RegularInstruction();

    ASSERT_EQ(0x2345, cpu.Get_pc());
}
