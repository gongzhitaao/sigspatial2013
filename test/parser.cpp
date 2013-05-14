#include <gtest/gtest.h>

#include <algorithm>
#include <functional>
#include <random>
#include <string>
#include <vector>

#include "../gmlparser.h"

const int TEST_CASES = 100;

char *make_gml_point(char *s, double x, double y)
{
    sprintf(s,
            "<gml:Point srsName=\"EPSG:54004"
            "xmlns:gml=\"http://www.opengis.net/gml\">"
            "<gml:coordinates decimal=\".\" cs=\",\" ts=\" \">"
            "%f,%f</gml:coordinates></gml:Point>",
            x, y);
    return s;
}

TEST(parser, PointParserTest)
{
    double a = -1.0, b = 1.0;
    std::default_random_engine eng;
    std::uniform_real_distribution<double> unif(a, b);

    GMLParser _gnp;

    char pt[256];
    for (int i = 0; i < TEST_CASES; ++i) {
        double x(unif(eng)), y(unif(eng)), xx, yy;
        std::cout << "heo" << std::endl;

        _gnp.point(make_gml_point(pt, x, y), xx, yy);
        std::cout << "world" << std::endl;
        ASSERT_DOUBLE_EQ(x, xx) << "x differs at index " << i;
        ASSERT_DOUBLE_EQ(y, yy) << "y differs at index " << i;
    }
}

TEST(parser, leak)
{
}
