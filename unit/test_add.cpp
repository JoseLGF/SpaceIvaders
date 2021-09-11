/*
 * Test file for add group instructions
 *
 */

#include <string>
#include <gtest/gtest.h>

using namespace std;

TEST(ccm, TestingTheGTestFramework) {
    ASSERT_EQ(1, 2-1);
}


int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}
