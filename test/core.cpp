#include <gtest/gtest.h>

#include <fstream>
#include <iostream>
#include <iterator>
#include <set>
#include <string>

#include "../core.h"

struct item
{
    int i[4];
};

bool operator < (const item &a, const item &b)
{
    return a.i[0] > b.i[0] ? false :
        a.i[0] < b.i[0] ? true :
        a.i[1] > b.i[1] ? false :
        a.i[1] < b.i[1] ? true :
        a.i[2] > b.i[2] ? false :
        a.i[2] < b.i[2] ? true :
        a.i[3] > b.i[3] ? false :
        a.i[3] < b.i[3] ? true : false;
}

bool operator == (const item &a, const item &b)
{
    return a.i[0] == b.i[0] &&
        a.i[1] == b.i[1] &&
        a.i[2] == b.i[2] &&
        a.i[3] == b.i[3];
}


std::ostream &operator << (std::ostream &o, const item &i)
{
    o << i.i[0] << ':' << i.i[1] << ":" << i.i[2] << ':' << i.i[3];
    return o;
}


TEST(core, ver2)
{
    std::string point_input("../in/points500.txt");
    std::string poly_input("../in/poly10.txt");
    std::string out0("../in/polys10_points500_INSIDE_out");
    std::string out1("../out/polys10_points500_INSIDE_out");

    ver2(point_input, poly_input, out1);
    std::cout << "helo" << std::endl;

    item f;
    char ch;

    std::set<item> s0;
    std::ifstream f0(out0);
    while (f0 >> f.i[0] >> ch >> f.i[1] >>
           ch >> f.i[2] >> ch >> f.i[3]) s0.insert(f);
    f0.close();

    std::set<item> s1;
    std::ifstream f1(out1);
    while (f1 >> f.i[0] >> ch >> f.i[1] >>
           ch >> f.i[2] >> ch >> f.i[3]) s1.insert(f);
    f1.close();

    // ASSERT_EQ(s0.size(), s1.size()) << "even size not match" << std::endl;

    for (std::set<item>::iterator i = s1.begin(); i != s1.end(); ++i)
        ASSERT_TRUE(s0.end() != s0.find(*i))
            << '\n' << *i << ':'
            << "\nexpected size: " << s0.size()
            << "\nactual size: " << s1.size() << std::endl;
}
