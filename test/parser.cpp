#include <gtest/gtest.h>

#include <algorithm>
#include <functional>
#include <random>
#include <string>
#include <vector>

#include <boost/lexical_cast.hpp>

#include "../gmlparser.h"
#include "../utils.h"

const int TEST_CASES = 100;

TEST(parser, PointParserTest)
{
    double a = -1.0, b = 1.0;
    std::default_random_engine eng;
    std::uniform_real_distribution<double> unif(a, b);

    GMLParser _gnp;

    for (int i = 0; i < TEST_CASES; ++i) {
        double x(unif(eng)), y(unif(eng)), xx, yy;
        _gnp.point(make_gml_point(x, y).c_str(), xx, yy);
        ASSERT_DOUBLE_EQ(x, xx) << "x differs at index " << i;
        ASSERT_DOUBLE_EQ(y, yy) << "y differs at index " << i;
    }
}
