/*
 * Test file for stack group instructions
 *
 */

#include <string>
#include <gtest/gtest.h>

#include "../include/i8080.h"

using namespace std;

TEST(StackGroup, PUSH_D_VerifyNormalAddition) {
    CPU_8080 cpu;
    cpu.Initialize();
    cpu.Set_d(0xde);
    cpu.Set_e(0xad);
    cpu.Set_sp(0x2020);
    cpu.WriteMemoryAt(0x0000, 0xd5); // PUSH_D instruction

    cpu.EmulateCycle();

    ASSERT_EQ(0xde, cpu.ReadMemoryAt(0x201f));
    ASSERT_EQ(0xad, cpu.ReadMemoryAt(0x201e));
    ASSERT_EQ(0x201e, cpu.Get_sp());
    ASSERT_EQ(0x0001, cpu.Get_pc());
}

TEST(StackGroup, PUSH_H_VerifyNormalAddition) {
    CPU_8080 cpu;
    cpu.Initialize();
    cpu.Set_h(0xde);
    cpu.Set_l(0xad);
    cpu.Set_sp(0x2020);
    cpu.WriteMemoryAt(0x0000, 0xe5); // PUSH_H instruction

    cpu.EmulateCycle();

    ASSERT_EQ(0xde, cpu.ReadMemoryAt(0x201f));
    ASSERT_EQ(0xad, cpu.ReadMemoryAt(0x201e));
    ASSERT_EQ(0x201e, cpu.Get_sp());
    ASSERT_EQ(0x0001, cpu.Get_pc());
}

TEST(StackGroup, PUSH_B_VerifyNormalAddition) {
    CPU_8080 cpu;
    cpu.Initialize();
    cpu.Set_b(0xde);
    cpu.Set_c(0xad);
    cpu.Set_sp(0x2020);
    cpu.WriteMemoryAt(0x0000, 0xc5); // PUSH_B instruction

    cpu.EmulateCycle();

    ASSERT_EQ(0xde, cpu.ReadMemoryAt(0x201f));
    ASSERT_EQ(0xad, cpu.ReadMemoryAt(0x201e));
    ASSERT_EQ(0x201e, cpu.Get_sp());
    ASSERT_EQ(0x0001, cpu.Get_pc());
}

TEST(StackGroup, POP_H_VerifyNormalOperation) {
    CPU_8080 cpu;
    cpu.Initialize();

    // Precondition
    cpu.WriteMemoryAt(0x1239, 0x3d);
    cpu.WriteMemoryAt(0x123a, 0x93);
    cpu.Set_h(0x0f);
    cpu.Set_l(0xf0);
    cpu.Set_sp(0x1239);
    cpu.WriteMemoryAt(0x0000, 0xe1); // POP_H instruction

    cpu.EmulateCycle();

    ASSERT_EQ(0x93, cpu.Get_h());
    ASSERT_EQ(0x3d, cpu.Get_l());
    ASSERT_EQ(0x123b, cpu.Get_sp());
    ASSERT_EQ(0x0001, cpu.Get_pc());
}

TEST(StackGroup, POP_B_VerifyNormalOperation) {
    CPU_8080 cpu;
    cpu.Initialize();

    // Precondition
    cpu.WriteMemoryAt(0x1239, 0x3d);
    cpu.WriteMemoryAt(0x123a, 0x93);
    cpu.Set_b(0x0f);
    cpu.Set_c(0xf0);
    cpu.Set_sp(0x1239);
    cpu.WriteMemoryAt(0x0000, 0xc1); // POP_B instruction

    cpu.EmulateCycle();

    ASSERT_EQ(0x93, cpu.Get_b());
    ASSERT_EQ(0x3d, cpu.Get_c());
    ASSERT_EQ(0x123b, cpu.Get_sp());
    ASSERT_EQ(0x0001, cpu.Get_pc());
}

TEST(StackGroup, POP_D_VerifyNormalOperation) {
    CPU_8080 cpu;
    cpu.Initialize();

    // Precondition
    cpu.WriteMemoryAt(0x1239, 0x3d);
    cpu.WriteMemoryAt(0x123a, 0x93);
    cpu.Set_d(0x0f);
    cpu.Set_e(0xf0);
    cpu.Set_sp(0x1239);
    cpu.WriteMemoryAt(0x0000, 0xd1); // POP_D instruction

    cpu.EmulateCycle();

    ASSERT_EQ(0x93, cpu.Get_d());
    ASSERT_EQ(0x3d, cpu.Get_e());
    ASSERT_EQ(0x123b, cpu.Get_sp());
    ASSERT_EQ(0x0001, cpu.Get_pc());
}
