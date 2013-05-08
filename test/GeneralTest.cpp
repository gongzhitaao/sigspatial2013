#include <gtest/gtest.h>

#include <algorithm>
#include <functional>
#include <random>
using namespace std;

#include <boost/lexical_cast.hpp>

TEST(general, DoubleStringConversion)
{
    double a = -1.0, b = 1.0;
    default_random_engine eng(4);
    uniform_real_distribution<double> unif(a, b);

    for (int i = 0; i < 100; ++i) {
        double d = unif(eng);
        string s = boost::lexical_cast<string>(d);
        double dd = boost::lexical_cast<double>(s);
        ASSERT_DOUBLE_EQ(d, dd) << "d differs at index " << i << endl;
    }
}

TEST(general, VectorEqual)
{
    vector<double> x(10), y(10);

    double a = -1.0, b = 1.0;
    default_random_engine eng;
    uniform_real_distribution<double> unif(a, b);

    auto generator = bind(unif, eng);
    generate_n(x.begin(), 10, generator);
    copy(x.begin(), x.end(), y.begin());

    ASSERT_EQ(x, y);
}