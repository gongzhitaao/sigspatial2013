#include <gtest/gtest.h>

#include <algorithm>
#include <chrono>
#include <random>

#include <boost/lexical_cast.hpp>

const int TEST_CASE = 100;

char *foo()
{
    char a[12];
    snprintf(a, 12, "helo world");
    return a;
}

TEST(general, foo)
{
    char *ch = foo();
    std::cout << ch << std::endl;
}
