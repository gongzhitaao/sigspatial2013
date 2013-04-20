#include <gtest/gtest.h>

#include <vector>
using namespace std;

#include "Core.h"

TEST(core, pip)
{
    // square
    //double xx[] = {0, 1, 1, 0};
    //double yy[] = {0, 0, 1, 1};

    double xx[] = {0, 1, 1, 0, .5, .2, 0};
    double yy[] = {0, 0, 1, 1, .5, .5, 0};
    vector<double> rx(begin(xx), end(xx)), ry(begin(yy), end(yy));
    EXPECT_FALSE(pip(-1.31800368974221E7, 3995712.25191062, rx, ry));
}