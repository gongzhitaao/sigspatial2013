#include "stdafx.h"
#include "CppUnitTest.h"

#include <limits>
#include <random>
#include <string>
using namespace std;

#include <gtest/gtest.h>

#include "GMLNaiveParser.h"
#include "Generator.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest
{		
    TEST_CLASS(GMLNaiveParserTest)
    {
    public:

        TEST_METHOD(PointParserTest)
        {
            double a = -1.0, b = 1.0;
            default_random_engine eng;
            uniform_real_distribution<double> unif(a, b);

            string s;
            double xx, yy;
            for (int i = 0; i < TEST_CASES; ++i) {
                double x(unif(eng)), y(unif(eng));
                _gnp.point(_gtr.make_gml_point(s, x, y).c_str(), xx, yy);
                ASSERT_DOUBLE_EQ(x, xx);
                ASSERT_DOUBLE_EQ(y, yy);
            }
        }

        TEST_METHOD(PolygonParser)
        {
            double a = -1.0, b = 1.0;
            default_random_engine eng;
            uniform_real_distribution<double> unif(a, b);
        }

    private:
        enum {TEST_CASES = 100};

        GMLNaiveParser _gnp;
        Generator _gtr;
    };
}