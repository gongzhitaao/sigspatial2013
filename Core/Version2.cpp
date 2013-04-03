// Version1.cpp

#include "Core.h"
using namespace concurrency;

#include "GMLNaiveParser.h"
#include "FloatingPoint.h"

#include <cmath>
#include <iostream>
#include <vector>
using namespace std;

bool pip(double x, double y,
         const vector<double> &rx, const vector<double> &ry)
{
    int w = 0;
    for (int i = 0; i < rx.size() - 1; ++i) {
        double x0 = rx[i] - x, y0 = ry[i] - y; 
        double x1 = rx[i+1] - x, y1 = ry[i+1] - y;
        if (y0 * y1 < 0) {
            if (x0 + y0*(x1-x0)/(y0-y1) > 0)
                if (y0 < 0) w += 2; else w -= 2;
        } else if (FloatingPoint<double>(y0).AlmostEquals(FloatingPoint<double>(0)) && x0 >= 0) {
            if (y1 > 0) ++w; else if (y1 < 0) --w;
        } else if (FloatingPoint<double>(y1).AlmostEquals(FloatingPoint<double>(0)) && x1 >= 0) {
            if (y0 < 0) ++w; else if (y0 > 0)--w;
        }
    }
    return w;
}

bool inside(double x, double y, Polygon &poly)
{
    if (!poly.might_contain(x, y) ||
        !pip(x, y, poly.ox, poly.oy)) return false;

    int n = poly.ix.size();
    for (int i = 0; i < n; ++i)
        if (poly.might_not_contain(x, y, i) && 
            pip(x, y, poly.ix[i], poly.iy[i]))
            return false;

    return true;
}

void version2(const string &fpt, const string &fpoly, const string &o)
{
    ofstream fout(o);

    string xmlstr;
    GMLNaiveParser gnp;
    Points pts;
    vector<Polygon> polys;

    parallel_invoke(
        [&, fpt] {
            char ch;
            int id, seq;
            double x, y;
            ifstream fin_point(fpt);
            while (fin_point >> ch) {
                while (':' != ch) fin_point >> ch;
                fin_point >> id >> ch >> seq >> ch;
                getline(fin_point, xmlstr);
                gnp.point(xmlstr.c_str(), x, y);
                pts.id.push_back(id);
                pts.seq.push_back(seq);
                pts.x.push_back(x);
                pts.y.push_back(y);
            }
            fin_point.close();
    },
        [&, fpoly] {
            char ch;
            ifstream fin_poly(fpoly);
            while (fin_poly >> ch) {
                Polygon poly;
                while (':' != ch) fin_poly >> ch;
                fin_poly >> poly.id >> ch >> poly.seq >> ch;
                getline(fin_poly, xmlstr);
                gnp.polygon(xmlstr.c_str(), poly);
                polys.push_back(poly);
            }
            fin_poly.close();
    },
        [] {
    });

    for (int i = 0; i < pts.x.size(); ++i) {
        for (int j = 0; j < polys.size(); ++j) {
            if (pts.seq[i] >= polys[j].seq) {
                if (inside(pts.x[i], pts.y[i], polys[j]))
                    fout << pts.id[i] << ':' << pts.seq[i] << "::" 
                    << polys[j].id << ':' << polys[j].seq << endl;
            }
        }
    }
    fout.close();
}
