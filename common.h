#pragma once

#include <vector>

struct Point
{
    int id, seq;
    double coord[2];

    Point (int a, int b, double c, double d)
        : id(a), seq(b) { coord[0] = c, coord[1] = d; }
};

struct Polygon
{
    std::vector<double> ox, oy;
    std::vector<std::vector<double> > ix, iy;

    double oxx[2], oyy[2];
    std::vector<double> ixa, ixb, iya, iyb;

    int id, seq;

    bool might_contain(double x, double y) {
        return oxx[0] < x && x < oxx[1] && oyy[0] < y && y < oyy[1];
    }

    bool might_not_contain(double x, double y, int i) {
        return ixa[i] < x && x < ixb[i] && iya[i] < y && y < iyb[i];
    }
};
