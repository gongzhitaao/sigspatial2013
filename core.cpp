// Version2.cpp

#include "core.h"

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include <tbb/parallel_invoke.h>

#include "common.h"
#include "FloatingPoint.h"
#include "gmlparser.h"

//bool pip(double x, double y,
//         const std::vector<double> &rx, const std::vector<double> &ry)
//{
//    int w = 0;
//    for (int i = 0; i < rx.size() - 1; ++i) {
//        double x0 = rx[i] - x, y0 = ry[i] - y;
//        double x1 = rx[i+1] - x, y1 = ry[i+1] - y;
//        if (y0 * y1 < 0) {
//            if (x0 + y0*(x1-x0)/(y0-y1) > 0)
//                if (y0 < 0) w += 2; else w -= 2;
//        } else if (FloatingPoint<double>(y0).AlmostEquals(FloatingPoint<double>(0)) && x0 >= 0) {
//            if (y1 > 0) ++w; else if (y1 < 0) --w;
//        } else if (FloatingPoint<double>(y1).AlmostEquals(FloatingPoint<double>(0)) && x1 >= 0) {
//            if (y0 < 0) ++w; else if (y0 > 0)--w;
//        }
//    }
//    return w;
//}

//bool inside(double x, double y, Polygon &poly)
//{
//    if (!poly.might_contain(x, y) ||
//        !pip(x, y, poly.ox, poly.oy)) return false;
//
//    int n = poly.ix.size();
//    for (int i = 0; i < n; ++i)
//        if (poly.might_not_contain(x, y, i) &&
//            pip(x, y, poly.ix[i], poly.iy[i]))
//            return false;
//
//    return true;
//}

void version2(const std::string &fpt, const std::string &fpoly, const std::string &o)
{
    std::vector<Point> pts;
    std::vector<Polygon> polys;

    // tbb::parallel_invoke([&] {
    //         std::string xmlstr;
    //         GMLParser gnp;
    //         char ch;

    //         std::ifstream fin_poly(fpoly);
    //         while (fin_poly >> ch) {
    //             Polygon poly;
    //             while (':' != ch) fin_poly >> ch;
    //             fin_poly >> poly.id >> ch >> poly.seq >> ch;
    //             getline(fin_poly, xmlstr);
    //             gnp.polygon(xmlstr.c_str(), poly);
    //             polys.push_back(poly);
    //         }
    //         fin_poly.close();
    //     },
    //     [&] {
    //         std::string xmlstr;
    //         GMLParser gnp;
    //         char ch;

    //         std::ifstream fin_point(fpt);
    //         while (fin_point >> ch) {
    //             Point pt;
    //             while (':' != ch) fin_point >> ch;
    //             fin_point >> pt.id >> ch >> pt.seq >> ch;
    //             getline(fin_point, xmlstr);
    //             gnp.point(xmlstr.c_str(), pt);
    //             pts.push_back(pt);
    //         }
    //         fin_point.close();
    //     });
}
