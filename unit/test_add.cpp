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

    cpu.EmulateCycle();

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

    cpu.EmulateCycle();

    ASSERT_EQ(0xde, cpu.Get_h());
    ASSERT_EQ(0xac, cpu.Get_l());
    ASSERT_EQ(true, cpu.Get_cy());
    ASSERT_EQ(0x0001, cpu.Get_pc());
}
