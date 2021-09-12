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
