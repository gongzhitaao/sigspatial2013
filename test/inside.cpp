#include <gtest/gtest.h>

#include <chrono>
#include <fstream>
#include <iostream>
#include <iterator>
#include <set>
#include <string>

#include <boost/algorithm/string.hpp>

#include "../core.h"

TEST(core, insidev2)
{
    const int MAX_PATH = 128;
    const int POINTS = 1000;
    const int POLYS = 15;

    char point_input_file[MAX_PATH], poly_input_file[MAX_PATH],
        out0_file[MAX_PATH], out1_file[MAX_PATH];

    snprintf(point_input_file, MAX_PATH, "../in/points%d.txt", POINTS);
    snprintf(poly_input_file, MAX_PATH, "../in/poly%d.txt", POLYS);
    snprintf(out0_file, MAX_PATH,
             "../in/polys%d_points%d_INSIDE_out", POLYS, POINTS);
    snprintf(out1_file, MAX_PATH,
             "../out/polys%d_points%d_INSIDE_out", POLYS, POINTS);

    inside(point_input_file, poly_input_file, out1_file);

    std::string s;

    std::set<std::string> s0;
    std::ifstream f0(out0_file);

    while (f0 >> s) {
        boost::algorithm::trim(s);
        s0.insert(s);
    }
    f0.close();

    std::set<std::string> s1;
    std::ifstream f1(out1_file);
    while (f1 >> s) {
        boost::algorithm::trim(s);
        s1.insert(s);
    }
    f1.close();

    for (std::set<std::string>::iterator i = s1.begin(); i != s1.end(); ++i)
        ASSERT_TRUE(s0.end() != s0.find(*i))
            << '\n' << *i << ':'
            << "\nexpected size: " << s0.size()
            << "\nactual size: " << s1.size() << std::endl;

    ASSERT_TRUE(s0.size() == s1.size())
        << "\nsome points missing!"
        << "\nexpected size: " << s0.size()
        << "\nactual size: " << s1.size() << std::endl;

}
