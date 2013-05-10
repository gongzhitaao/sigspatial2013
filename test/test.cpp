#include <iostream>
#include <gtest/gtest.h>

GTEST_API_ int main(int argc, char *argv[]) {
    ::testing::GTEST_FLAG(filter) = "core.ver2";
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
