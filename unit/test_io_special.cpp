/*
 * Test file for io and special group instructions
 *
 */

#include <string>
#include <gtest/gtest.h>

#include "../include/i8080.h"

using namespace std;

TEST(IoSpecialGroup, STC_VerifyCarryIsSet) {
    CPU_8080 cpu;
    cpu.Initialize();
    cpu.Set_cy(false);
    cpu.WriteMemoryAt(0x0000, 0x37); // STC instruction

    cpu.RegularInstruction();

    ASSERT_EQ(true, cpu.Get_cy());
    ASSERT_EQ(0x0001, cpu.Get_pc());
}

