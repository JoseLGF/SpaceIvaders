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

TEST(IoSpecialGroup, CMA) {
    //
    // Before test
    // A        = 0x51
    // PC       = 0x0000
    // After test
    // A        = 0xae
    // PC       = 0x0001
    // Everything else unmodified
    CPU_8080 cpu;
    cpu.Initialize();
    cpu.Set_a(0x51);
    cpu.WriteMemoryAt(0x0000, 0x2f); // CMA instruction

    cpu.RegularInstruction();

    cpu.Set_a(0xae);
    ASSERT_EQ(0x0001, cpu.Get_pc());
}

