/*
 * Test file for add group instructions
 *
 */

#include <string>
#include <gtest/gtest.h>

#include "../include/i8080.h"

using namespace std;

TEST(AddGroup, DAD_D_VerifyNormalAddition) {
    CPU_8080 cpu;
    cpu.Initialize();
    cpu.Set_d(0xde);
    cpu.Set_e(0xad);
    cpu.Set_h(0x12);
    cpu.Set_l(0x34);
    cpu.WriteMemoryAt(0x0000, 0x19); // DAD_D instruction

    cpu.RegularInstruction();

    ASSERT_EQ(0xf0, cpu.Get_h());
    ASSERT_EQ(0xe1, cpu.Get_l());
    ASSERT_EQ(false, cpu.Get_cy());
    ASSERT_EQ(0x0001, cpu.Get_pc());
}

TEST(AddGroup, DAD_D_VerifyWrapAroundFfff) {
    CPU_8080 cpu;
    cpu.Initialize();
    cpu.Set_d(0xff);
    cpu.Set_e(0xff);
    cpu.Set_h(0xde);
    cpu.Set_l(0xad);
    cpu.WriteMemoryAt(0x0000, 0x19); // DAD_D instruction

    cpu.RegularInstruction();

    ASSERT_EQ(0xde, cpu.Get_h());
    ASSERT_EQ(0xac, cpu.Get_l());
    ASSERT_EQ(true, cpu.Get_cy());
    ASSERT_EQ(0x0001, cpu.Get_pc());
}

TEST(AddGroup, DAD_H_VerifyWrapAroundFfff) {
    CPU_8080 cpu;
    cpu.Initialize();
    cpu.Set_h(0xde);
    cpu.Set_l(0xad);
    cpu.WriteMemoryAt(0x0000, 0x29); // DAD_H instruction

    cpu.RegularInstruction();

    ASSERT_EQ(0xbd, cpu.Get_h());
    ASSERT_EQ(0x5a, cpu.Get_l());
    ASSERT_EQ(true, cpu.Get_cy());
    ASSERT_EQ(0x0001, cpu.Get_pc());
}

TEST(AddGroup, DAD_B_VerifyWrapAroundFfff) {
    CPU_8080 cpu;
    cpu.Initialize();
    cpu.Set_b(0xff);
    cpu.Set_c(0xff);
    cpu.Set_h(0xde);
    cpu.Set_l(0xad);
    cpu.WriteMemoryAt(0x0000, 0x09); // DAD_B instruction

    cpu.RegularInstruction();

    ASSERT_EQ(0xde, cpu.Get_h());
    ASSERT_EQ(0xac, cpu.Get_l());
    ASSERT_EQ(true, cpu.Get_cy());
    ASSERT_EQ(0x0001, cpu.Get_pc());
}

TEST(AddGroup, ADI) {
    CPU_8080 cpu;
    cpu.Initialize();
    cpu.Set_a(0xbb);
    cpu.WriteMemoryAt(0x0000, 0xc6); // ADI instruction
    cpu.WriteMemoryAt(0x0001, 0xf2);

    cpu.RegularInstruction();

    ASSERT_EQ(0xad, cpu.Get_a());
    ASSERT_EQ(true, cpu.Get_cy());
    ASSERT_EQ(false, cpu.Get_z());
    ASSERT_EQ(false, cpu.Get_p());
    ASSERT_EQ(0x0002, cpu.Get_pc());
}
