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

TEST(parser, PolygonParserTest_NoHoles)
{
    GMLNaiveParser _gnp;
    Generator _gtr;

    string s;
    vector<double> ox(10), oy(10), ixt, iyt;
    vector<double> oxx, oyy;
    vector<vector<double> > ix, iy, ixx, iyy;

    for (int i = 0; i < TEST_CASES; ++i) {
        _gtr.fill_vec_real_n(ox, 10);
        _gtr.fill_vec_real_n(oy, 10);

        _gnp.polygon(_gtr.make_gml_poly(s, ox, oy, ix, iy).c_str(), oxx, oyy, ixx, iyy);
        for (int i = 0; i < ox.size(); ++i) {
            ASSERT_DOUBLE_EQ(ox[i], oxx[i]);
            ASSERT_DOUBLE_EQ(oy[i], oyy[i]);
        }
    }
}

TEST(parser, PlygonParserTest_WithHoles)
{
    GMLNaiveParser _gnp;
    Generator _gtr;
    
    string s;
    vector<double> ox(10), oy(10), ixt(10), iyt(10);
    vector<double> oxx, oyy;
    vector<vector<double> > ix, iy, ixx, iyy;

    for (int i = 0; i < TEST_CASES; ++i) {
        _gtr.fill_vec_real_n(ox, 10);
        _gtr.fill_vec_real_n(oy, 10);
        for (int j = 0; j < TEST_CASES; ++j) {
            _gtr.fill_vec_real_n(ixt, 10);
            _gtr.fill_vec_real_n(iyt, 10);
            ix.push_back(ixt);
            iy.push_back(iyt);
            ixt.clear();
            iyt.clear();
        }
    }

    _gnp.polygon(_gtr.make_gml_poly(s, ox, oy, ix, iy).c_str(), oxx, oyy, ixx, iyy);

    for (int i = 0; i < ox.size(); ++i) {
        ASSERT_DOUBLE_EQ(ox[i], oxx[i]);
        ASSERT_DOUBLE_EQ(oy[i], oyy[i]);
    }

    for (int i = 0; i < ix.size(); ++i) {
        for (int j = 0; j < ix[i].size(); ++j) {
            ASSERT_DOUBLE_EQ(ix[i][j], ixx[i][j]);
            ASSERT_DOUBLE_EQ(iy[i][j], iyy[i][j]);
        }
    }
}