#include <gtest/gtest.h>

#include <algorithm>
#include <functional>
#include <random>
#include <vector>
using namespace std;

#include "Generator.h"
#include "GMLNaiveParser.h"

const int TEST_CASES = 100;

TEST(parser, PointParserTest)
{
    double a = -1.0, b = 1.0;
    default_random_engine eng;
    uniform_real_distribution<double> unif(a, b);

    GMLNaiveParser _gnp;
    Generator _gtr;

    string s;
    double xx, yy;
    for (int i = 0; i < TEST_CASES; ++i) {
        double x(unif(eng)), y(unif(eng));
        _gnp.point(_gtr.make_gml_point(s, x, y).c_str(), xx, yy);
        ASSERT_DOUBLE_EQ(x, xx) << "x differs at index " << i;
        ASSERT_DOUBLE_EQ(y, yy) << "y differs at index " << i;
    }
}