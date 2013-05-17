#include <gtest/gtest.h>

#include <string>
#include <sstream>
#include <vector>

#include <boost/lexical_cast.hpp>

#include "testclock.h"

TEST(stree, string2double)
{
    double a = -1.0, b = 1.0;
    std::random_device rd;
    std::mt19937 eng1(rd());
    std::mt19937 eng2(rd());
    std::uniform_real_distribution<double> unif(a, b);
    auto gen1 = std::bind(unif, eng1);
    auto gen2 = std::bind(unif, eng2);

    const std::size_t MAX = 1e6;

    std::vector<double> vx(MAX), vy(MAX);
    std::generate(vx.begin(), vx.end(), gen1);
    std::generate(vy.begin(), vy.end(), gen2);

    std::vector<std::string> vs(vx.size());
    std::transform(vx.begin(), vx.end(), vy.begin(), vs.begin(),
                   [](double a, double b) {
                       return boost::lexical_cast<std::string>(a)
                           + "," + boost::lexical_cast<std::string>(b);
                   });

    Clock clk;
    char ch;
    for (std::size_t i = 0; i < vs.size(); ++i) {
        std::stringstream ss(vs[i]);
        double x, y;
        ss >> x >> ch >> y;
        // ASSERT_DOUBLE_EQ(vx[i], x);
        // ASSERT_DOUBLE_EQ(vy[i], y);
    }
    float e1 = clk.elapsed();

    clk.reset();
    std::string::size_type sz;
    for (std::size_t i = 0; i < vs.size(); ++i) {
        double x = std::stod(vs[i], &sz);
        double y = std::stod(vs[i].substr(sz+1));
        // ASSERT_DOUBLE_EQ(vx[i], x);
        // ASSERT_DOUBLE_EQ(vy[i], y);
    }
    float e2 = clk.elapsed();

    clk.reset();
    for (std::size_t i = 0; i < vs.size(); ++i) {
        const char *s = vs[i].c_str();
        char *p;
        double x = strtod(s, &p);
        double y = strtod(p+1, NULL);
        // ASSERT_DOUBLE_EQ(vx[i], x);
        // ASSERT_DOUBLE_EQ(vy[i], y);
    }
    float e3 = clk.elapsed();

    clk.reset();
    for (std::size_t i = 0; i < vs.size(); ++i) {
        int j = 0;
        while (vs[i][j++] != ',') ;
        double x = boost::lexical_cast<double>(vs[i].substr(0, j-1));
        double y = boost::lexical_cast<double>(vs[i].substr(j));
        // ASSERT_DOUBLE_EQ(vx[i], x);
        // ASSERT_DOUBLE_EQ(vy[i], y);
    }
    float e4 = clk.elapsed();

    std::cout << "ratio of stringstream/strod: " << (e1/e3) << std::endl;
    std::cout << "ratio of stod/strod: " << (e2/e3) << std::endl;
    std::cout << "ratio of lexical_cast/strod: " << (e4/e3) << std::endl;
}
