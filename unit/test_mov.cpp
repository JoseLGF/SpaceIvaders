/*
 * Test file for move group instructions
 *
 */

#include <string>
#include <gtest/gtest.h>

#include "../include/i8080.h"

TEST(MoveGroup, MOV_M_A_VerifyContentsOfACorreclyLoaded) {
    CPU_8080 cpu;
    cpu.Initialize();
    cpu.Set_h(0x01);
    cpu.Set_l(0xd0);
    cpu.Set_a(0xad);
    cpu.WriteMemoryAt(0x0000, 0x77); // MOV_M_A instruction

    cpu.EmulateCycle();

    ASSERT_EQ(0xad, cpu.ReadMemoryAt(0x01d0));
    ASSERT_EQ(0x01, cpu.Get_pc());
}

TEST(MoveGroup, STA_VerifyContentsOfACorrectlyLoaded) {
    CPU_8080 cpu;
    cpu.Initialize();
    cpu.Set_a(0xcc);
    cpu.WriteMemoryAt(0x0000, 0x32); // STA instruction
    cpu.WriteMemoryAt(0x0001, 0x45); // adr lo byte
    cpu.WriteMemoryAt(0x0002, 0x23); // adr hi byte

    cpu.EmulateCycle();

    ASSERT_EQ(0xcc, cpu.ReadMemoryAt(0x2345));
    ASSERT_EQ(0x03, cpu.Get_pc());
}

TEST(MoveGroup, LDA_VerifyACorrectlyLoaded) {
    CPU_8080 cpu;
    cpu.Initialize();
    cpu.Set_a(0x00);
    cpu.WriteMemoryAt(0x0000, 0x3a); // STA instruction
    cpu.WriteMemoryAt(0x0001, 0x45); // adr lo byte
    cpu.WriteMemoryAt(0x0002, 0x23); // adr hi byte
    cpu.WriteMemoryAt(0x2345, 0xcc);

    cpu.EmulateCycle();

    ASSERT_EQ(0xcc, cpu.Get_a());
    ASSERT_EQ(0x03, cpu.Get_pc());
}

TEST(MoveGroup, MVI_M_VerifyContentsCorrectlyTransferred) {
    CPU_8080 cpu;
    cpu.Initialize();
    cpu.Set_h(0x12);
    cpu.Set_l(0x34);

    ASSERT_EQ(0x00, cpu.ReadMemoryAt(0x1234));

    cpu.WriteMemoryAt(0x0000, 0x36); // MVI_M instruction
    cpu.WriteMemoryAt(0x0001, 0xcc); // Operand

    cpu.EmulateCycle();

    ASSERT_EQ(0xcc, cpu.ReadMemoryAt(0x1234));
    ASSERT_EQ(0x02, cpu.Get_pc());
}

TEST(MoveGroup, XCHG_CheckValuesHaveBeenSwapped) {
    CPU_8080 cpu;
    cpu.Initialize();
    cpu.Set_h(0xde);
    cpu.Set_l(0xad);
    cpu.Set_d(0xbe);
    cpu.Set_e(0xef);

    cpu.WriteMemoryAt(0x0000, 0xeb); // XCHG instruction

    cpu.EmulateCycle();

    ASSERT_EQ(0xbe, cpu.Get_h());
    ASSERT_EQ(0xef, cpu.Get_l());
    ASSERT_EQ(0xde, cpu.Get_d());
    ASSERT_EQ(0xad, cpu.Get_e());
}
