#include <gtest/gtest.h>

#include <fstream>
#include <iterator>
#include <set>
#include <string>
using namespace std;

#include "Core.h"

struct foo
{
    int i[4];
};

bool operator<(const foo &a, const foo &b)
{
    return a.i[0] > b.i[0] ? false
        : a.i[0] < b.i[0] ? true
        : a.i[1] > b.i[1] ? false
        : a.i[1] < b.i[1] ? true
        : a.i[2] > b.i[2] ? false
        : a.i[2] < b.i[2] ? true
        : a.i[3] > b.i[3] ? false
        : a.i[3] < b.i[3] ? true : false;
}

bool operator==(const foo &a, const foo &b)
{
    return a.i[0] == b.i[0] && a.i[1] == b.i[1] && a.i[2] == b.i[2] && a.i[3] == b.i[3];
}

ostream &operator<<(ostream &o, const foo &f)
{
    o << f.i[0] << ':' << f.i[1] << ":" << f.i[2] << ':' << f.i[3];
    return o;
}

struct Index
{
    int id, seq;
};

bool operator<(const Index &a, const Index &b)
{
    return a.id < b.id ? true
        : a.id > b.id ? false
        : a.seq < b.seq ? true
        : a.seq > b.seq ? false : true;
}

TEST(core, Version2)
{
    string point_input("D:\\Documents\\Visual Studio 2012\\Projects\\SigSpatial2013\\TrainingDataSet\\points500.txt");
    string poly_input("D:\\Documents\\Visual Studio 2012\\Projects\\SigSpatial2013\\TrainingDataSet\\poly10.txt");
    string out0("D:\\Documents\\Visual Studio 2012\\Projects\\SigSpatial2013\\TrainingDataSet\\standard_output\\polys10_points500_INSIDE_out");
    string out1("D:\\Documents\\Visual Studio 2012\\Projects\\SigSpatial2013\\TrainingDataSet\\test_output\\polys10_points500_INSIDE_out");

    version2(point_input, poly_input, out1);

    ifstream f0(out0);
    set<foo> s0;
    foo f;
    char ch;
    while (f0 >> f.i[0] >> ch >> f.i[1] >> ch >> f.i[2] >> ch >> f.i[3])
        s0.insert(f);
    f0.close();

    ifstream f1(out1);
    set<foo> s1;
    while (f1 >> f.i[0] >> ch >> f.i[1] >> ch >> f.i[2] >> ch >> f.i[3])
        s1.insert(f);
    f1.close();

    //ASSERT_EQ(s0.size(), s1.size()) << "size not match";
    for (set<foo>::iterator i = s1.begin(); i != s1.end(); ++i)
        ASSERT_TRUE(s0.end() != s0.find(*i)) << *i << endl;
}