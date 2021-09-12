/*
 * Test file for logical gnd rotate group instructions
 *
 */

#include <string>
#include <gtest/gtest.h>

#include "../include/i8080.h"

using namespace std;

TEST(LogicalGroup, XRA_A_VerifyNormalOperation) {
    CPU_8080 cpu;
    cpu.Initialize();
    cpu.Set_a(0xf0);
    cpu.Set_cy(true);
    cpu.WriteMemoryAt(0x0000, 0xaf); // XRA_A instruction

    cpu.RegularInstruction();

    ASSERT_EQ(0x00, cpu.Get_a());
    ASSERT_EQ(false, cpu.Get_cy());
    ASSERT_EQ(0x0001, cpu.Get_pc());
}

TEST(LogicalGroup, ANA_A_VerifyNormalOperation) {
    CPU_8080 cpu;
    cpu.Initialize();
    cpu.Set_a(0xf0);
    cpu.Set_cy(true);
    cpu.Set_s(false);
    cpu.Set_p(false);
    cpu.Set_z(true);
    cpu.WriteMemoryAt(0x0000, 0xa7); // ANA_A instruction

    cpu.RegularInstruction();

    ASSERT_EQ(0xf0, cpu.Get_a());
    ASSERT_EQ(false, cpu.Get_cy());
    ASSERT_EQ(false, cpu.Get_z());
    ASSERT_EQ(true, cpu.Get_s());
    ASSERT_EQ(true, cpu.Get_p());
    ASSERT_EQ(0x0001, cpu.Get_pc());
}

TEST(LogicalGroup, CPI_VerifyEqualNumbers) {
    CPU_8080 cpu;
    cpu.Initialize();
    cpu.Set_a(0xf0);
    cpu.WriteMemoryAt(0x0000, 0xfe); // CPI instruction
    cpu.WriteMemoryAt(0x0001, 0xf0);

    cpu.RegularInstruction();

    ASSERT_EQ(false, cpu.Get_cy());
    ASSERT_EQ(true, cpu.Get_z());
    ASSERT_EQ(false, cpu.Get_s());
    ASSERT_EQ(true, cpu.Get_p());
    ASSERT_EQ(0x0002, cpu.Get_pc());
}

TEST(LogicalGroup, CPI_VerifyAIsGreater) {
    CPU_8080 cpu;
    cpu.Initialize();
    cpu.Set_a(0xf4);
    cpu.WriteMemoryAt(0x0000, 0xfe); // CPI instruction
    cpu.WriteMemoryAt(0x0001, 0xf0);

    cpu.RegularInstruction();

    ASSERT_EQ(false, cpu.Get_cy());
    ASSERT_EQ(false, cpu.Get_z());
    ASSERT_EQ(false, cpu.Get_s());
    ASSERT_EQ(false, cpu.Get_p());
    ASSERT_EQ(0x0002, cpu.Get_pc());
}

TEST(LogicalGroup, CPI_VerifyDataIsGreater) {
    CPU_8080 cpu;
    cpu.Initialize();
    cpu.Set_a(0xf0);
    cpu.WriteMemoryAt(0x0000, 0xfe); // CPI instruction
    cpu.WriteMemoryAt(0x0001, 0xf4);

    cpu.RegularInstruction();

    ASSERT_EQ(true, cpu.Get_cy());
    ASSERT_EQ(false, cpu.Get_z());
    ASSERT_EQ(true, cpu.Get_s());
    ASSERT_EQ(true, cpu.Get_p());
    ASSERT_EQ(0x0002, cpu.Get_pc());
}

TEST(LogicalGroup, RRC) {
    CPU_8080 cpu;
    cpu.Initialize();
    cpu.Set_a(0xf2);
    cpu.Set_cy(true);
    cpu.WriteMemoryAt(0x0000, 0x0f); // RRC instruction

    cpu.RegularInstruction();

    ASSERT_EQ(false, cpu.Get_cy());
    ASSERT_EQ(0x79, cpu.Get_a());
    ASSERT_EQ(0x0001, cpu.Get_pc());
}

TEST(LogicalGroup, ANI) {
    CPU_8080 cpu;
    cpu.Initialize();
    cpu.Set_a(0x3a);
    cpu.WriteMemoryAt(0x0000, 0xe6); // ANI instruction
    cpu.WriteMemoryAt(0x0001, 0x0f);

    cpu.RegularInstruction();

    ASSERT_EQ(false, cpu.Get_cy());
    ASSERT_EQ(false, cpu.Get_z());
    ASSERT_EQ(false, cpu.Get_s());
    ASSERT_EQ(true, cpu.Get_p());
    ASSERT_EQ(0x0a, cpu.Get_a());
    ASSERT_EQ(0x0002, cpu.Get_pc());
}
