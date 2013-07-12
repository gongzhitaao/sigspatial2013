#include <gtest/gtest.h>

#include <chrono>
#include <fstream>
#include <random>

#include "../common.h"

// TEST(memory, usage)
// {
//     const int MAX_PATH = 128;
//     const int POLYS = 15;

//     char point_input_file[MAX_PATH], poly_input_file[MAX_PATH],
//         out0_file[MAX_PATH], out1_file[MAX_PATH];

//     snprintf(poly_input_file, MAX_PATH, "../in/poly%d.txt", POLYS);
//     snprintf(out0_file, MAX_PATH,
//              "../test/memory");

//     double a = 1e4;
//     unsigned seed = std::chrono::system_clock::now()
//         .time_since_epoch().count();
//     std::mt19937 eng(seed);
//     std::uniform_real_distribution<double> unif(-a, a);

//     std::ofstream out0(out0_file);
//     Clock tmr;

//     for (int i = 1e6; i <= 1e6; i+=100) {

//         snprintf(point_input_file, MAX_PATH,
//                  "../test/points%d.txt", i);
//         snprintf(out1_file, MAX_PATH,
//                  "../test/polys%d_points%d_INSIDE_out", POLYS, i);

//         std::ofstream o(point_input_file);
//         for (int j = 0; j < i; ++j) {
//             std::string s = make_point(1, j+501, unif(eng), unif(eng));
//             o << s << std::endl;
//         }
//         o.close();

//         tmr.reset();
//         inside(point_input_file, poly_input_file, out1_file);

//         out0 << i << ' '
//              << getPeakRSS() << ' ' << getCurrentRSS() << ' '
//              << tmr.elapsed() << std::endl;

//     }
//     out0.close();

// }

TEST(memory, usage)
{
    using namespace SigSpatial2013;
    Interval w(Point_2(1.1, 1.2), Point_2(1.1, 1.2));
    std::cout << typeid(w.first).name() << std::endl;
    std::printf("%d %d\n", sizeof(w), sizeof(w.first));;
    std::printf("%f %f %f %f\n", w.first.x(), w.first.y(), w.second.x(), w.second.y());
}
