/*
 * Test file for call group instructions
 *
 */

#include <string>
#include <gtest/gtest.h>

#include "../include/i8080.h"

TEST(CallGroup, CALL_VerifyCorrectOperationsFromCall) {
    CPU_8080 cpu;
    cpu.Initialize();
    cpu.Set_sp(0x200f);
    cpu.WriteMemoryAt(0x0000, 0xcd); // CALL instruction
    cpu.WriteMemoryAt(0x0001, 0xcd);
    cpu.WriteMemoryAt(0x0002, 0xab);

    cpu.RegularInstruction();

    ASSERT_EQ(0xabcd, cpu.Get_pc());
    ASSERT_EQ(0x200d, cpu.Get_sp());
}

TEST(CallGroup, RET_VerifyCorrectOperationsFromRet) {
    CPU_8080 cpu;
    cpu.Initialize();
    cpu.Set_sp(0x200f);
    cpu.WriteMemoryAt(0x0000, 0xcd); // CALL instruction
    cpu.WriteMemoryAt(0x0001, 0x20);
    cpu.WriteMemoryAt(0x0002, 0x20);
    cpu.WriteMemoryAt(0x2020, 0xc9); // RET instruction

    cpu.RegularInstruction(); // Executes CALL
    cpu.RegularInstruction(); // Executes RET


    ASSERT_EQ(0x0003, cpu.Get_pc());
    ASSERT_EQ(0x200f, cpu.Get_sp());
}

TEST(CallGroup, RST0_VerifyPC) {
    CPU_8080 cpu;
    cpu.Initialize();
    cpu.Set_sp(0x200f);
    cpu.Set_pc(0x0080);
    cpu.WriteMemoryAt(0x0080, 0xc7); // RST0 instruction

    cpu.RegularInstruction();

    ASSERT_EQ(0x0000, cpu.Get_pc());
    ASSERT_EQ(0x200d, cpu.Get_sp());
}

TEST(CallGroup, RST5_VerifyPC) {
    CPU_8080 cpu;
    cpu.Initialize();
    cpu.Set_sp(0x200f);
    cpu.Set_pc(0x0080);
    cpu.WriteMemoryAt(0x0080, 0xef); // RST5 instruction

    cpu.RegularInstruction();

    ASSERT_EQ(0x0028, cpu.Get_pc());
    ASSERT_EQ(0x200d, cpu.Get_sp());
}
