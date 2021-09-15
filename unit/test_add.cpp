/*
 * Test file for add and subtract groups instructions
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

TEST(AddGroup, ADI_2) {
    CPU_8080 cpu;
    cpu.Initialize();
    cpu.Set_a(0x14);
    cpu.WriteMemoryAt(0x0000, 0xc6); // ADI instruction
    cpu.WriteMemoryAt(0x0001, 0x42);

    cpu.RegularInstruction();

    ASSERT_EQ(0x56, cpu.Get_a());
    ASSERT_EQ(false, cpu.Get_s());
    ASSERT_EQ(false, cpu.Get_cy());
    ASSERT_EQ(false, cpu.Get_z());
    ASSERT_EQ(true, cpu.Get_p());
    ASSERT_EQ(0x0002, cpu.Get_pc());

    cpu.WriteMemoryAt(0x0002, 0xc6); // Another ADI
    cpu.WriteMemoryAt(0x0003, 0xbe); // -66 (add negative number)

    cpu.RegularInstruction();

    ASSERT_EQ(0x14, cpu.Get_a());
    ASSERT_EQ(false, cpu.Get_s());
    ASSERT_EQ(true, cpu.Get_cy());
    ASSERT_EQ(false, cpu.Get_z());
    ASSERT_EQ(true, cpu.Get_p());
    ASSERT_EQ(0x0004, cpu.Get_pc());
}

TEST(AddGroup, ADI_AResultThatHasCarryButZeroAnswerWillAlsoSetZ) {
    CPU_8080 cpu;
    cpu.Initialize();
    cpu.Set_a(0xff);
    cpu.WriteMemoryAt(0x0000, 0xc6); // ADI instruction
    cpu.WriteMemoryAt(0x0001, 0x01);

    cpu.RegularInstruction();

    ASSERT_EQ(0x00, cpu.Get_a());       // A wraps to 0x00
    ASSERT_EQ(true, cpu.Get_cy());      // carry set
    ASSERT_EQ(true, cpu.Get_z());       // Zero set
    ASSERT_EQ(true, cpu.Get_p());      // parity true
    ASSERT_EQ(0x0002, cpu.Get_pc());
}

TEST(AddGroup, SUI_WhenAIsGreaterThanDataThenThereIsNoBorrow) {
    CPU_8080 cpu;
    cpu.Initialize();
    cpu.Set_a(0x34); // 0x34 = 52
    cpu.WriteMemoryAt(0x0000, 0xd6); // SUI instruction
    cpu.WriteMemoryAt(0x0001, 0x04);

    cpu.RegularInstruction();
    // 52 - 4 = 0x130, result has carry out set
    //
    // +52 = 0 0 1 1 0 1 0 0
    // - 4 = 1 1 1 1 1 1 0 0
    //       ---------------
    //     1 0 0 1 1 0 0 0 0
    //     |
    //     +--- Carry set, meaning number is in two's complement
    //          Therefore, carry must be reset

    ASSERT_EQ(0x30, cpu.Get_a());
    ASSERT_EQ(false, cpu.Get_cy());
    ASSERT_EQ(false, cpu.Get_z());
    ASSERT_EQ(false, cpu.Get_s());
    ASSERT_EQ(true, cpu.Get_p());
    ASSERT_EQ(0x0002, cpu.Get_pc());
}

TEST(AddGroup, SUI_WhenDataIsGreaterThanAThenThereIsBorrowAndUnderflow) {
    CPU_8080 cpu;
    cpu.Initialize();
    cpu.Set_a(0x0c);
    cpu.WriteMemoryAt(0x0000, 0xd6); // SUI instruction
    cpu.WriteMemoryAt(0x0001, 0x0f);

    cpu.RegularInstruction();
    // 12 - 15 = 0xff, with no borrow
    //
    // +12 = 0 0 0 0 1 1 0 0
    // -15 = 1 1 1 1 0 0 0 1
    //       ---------------
    //     0 1 1 1 1 1 1 0 1
    //     |
    //     +--- Carry reset, means no borrow, causing the carry to be set

    ASSERT_EQ(0xfd, cpu.Get_a());
    ASSERT_EQ(true, cpu.Get_cy());
    ASSERT_EQ(false, cpu.Get_z());
    ASSERT_EQ(true, cpu.Get_s());
    ASSERT_EQ(false, cpu.Get_p());
    ASSERT_EQ(0x0002, cpu.Get_pc());
}

TEST(AddGroup, ADD_M) {
    CPU_8080 cpu;
    cpu.Initialize();
    cpu.Set_a(0x6c);
    cpu.Set_h(0x23);
    cpu.Set_l(0x45);
    cpu.WriteMemoryAt(0x0000, 0x86); // ADD_M instruction
    cpu.WriteMemoryAt(0x2345, 0x2e);

    cpu.RegularInstruction();

    ASSERT_EQ(0x9a, cpu.Get_a());
    ASSERT_EQ(false, cpu.Get_cy());
    ASSERT_EQ(false, cpu.Get_z());
    ASSERT_EQ(true, cpu.Get_s());
    ASSERT_EQ(true, cpu.Get_p());
    ASSERT_EQ(0x0001, cpu.Get_pc());
}

TEST(AddGroup, SBI_WhenResultIsNegativeThenTheCarryBitIsSet) {
    CPU_8080 cpu;
    cpu.Initialize();
    cpu.Set_a(0x00);
    cpu.Set_cy(false);
    cpu.WriteMemoryAt(0x0000, 0xde); // SBI instruction
    cpu.WriteMemoryAt(0x0001, 0x01);

    cpu.RegularInstruction();

    ASSERT_EQ(0xff, cpu.Get_a());
    ASSERT_EQ(true, cpu.Get_cy());
    ASSERT_EQ(false, cpu.Get_z());
    ASSERT_EQ(true, cpu.Get_s());
    ASSERT_EQ(true, cpu.Get_p());
    ASSERT_EQ(0x0002, cpu.Get_pc());
}

TEST(AddGroup, SBI_WhenResultIsPositiveThenTheCarryBitIsReset) {
    CPU_8080 cpu;
    cpu.Initialize();
    cpu.Set_a(0x00);
    cpu.Set_cy(true);
    cpu.WriteMemoryAt(0x0000, 0xde); // SBI instruction
    cpu.WriteMemoryAt(0x0001, 0x01);

    cpu.RegularInstruction();

    ASSERT_EQ(0xfe, cpu.Get_a());
    ASSERT_EQ(true, cpu.Get_cy());
    ASSERT_EQ(false, cpu.Get_z());
    ASSERT_EQ(true, cpu.Get_s());
    ASSERT_EQ(false, cpu.Get_p());
    ASSERT_EQ(0x0002, cpu.Get_pc());
}

