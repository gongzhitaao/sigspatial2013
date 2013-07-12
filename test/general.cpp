#include <gtest/gtest.h>

#include <string>
#include <sstream>

struct Area
{
    double xa, xb, ya, yb;
    int ind;
};

struct Boundary
{
    Area corners[4];
    std::vector<Area> borders[4];
};


TEST(general, foo)
{
    std::cout << sizeof(Boundary) << std::endl;
}
