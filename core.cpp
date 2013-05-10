// Version2.cpp

#include "core.h"

// standard headers
#include <iostream>
#include <fstream>
#include <utility>
#include <string>
#include <vector>

// tbb for multi-processors
#include <tbb/parallel_invoke.h>
#include <tbb/concurrent_vector.h>

// cgal for range tree
#include <CGAL/Cartesian.h>

#include <CGAL/Bbox_2.h>
#include <CGAL/Polygon_2_algorithms.h>
#include <CGAL/Range_segment_tree_traits.h>
#include <CGAL/Range_tree_k.h>

#include "common.h"
#include "FloatingPoint.h"
#include "gmlparser.h"

// bool pip(double x, double y,
//          const std::vector<double> &rx, const std::vector<double> &ry)
// {
//     int w = 0;
//     for (size_t i = 0; i < rx.size() - 1; ++i) {
//         double x0 = rx[i] - x, y0 = ry[i] - y;
//         double x1 = rx[i+1] - x, y1 = ry[i+1] - y;
//         if (y0 * y1 < 0) {
//             if (x0 + y0*(x1-x0)/(y0-y1) > 0)
//                 if (y0 < 0) w += 2; else w -= 2;
//         } else if (FloatingPoint<double>(y0).AlmostEquals(FloatingPoint<double>(0)) && x0 >= 0) {
//             if (y1 > 0) ++w; else if (y1 < 0) --w;
//         } else if (FloatingPoint<double>(y1).AlmostEquals(FloatingPoint<double>(0)) && x1 >= 0) {
//             if (y0 < 0) ++w; else if (y0 > 0)--w;
//         }
//     }
//     return w;
// }

// bool inside(double x, double y, Polygon &poly)
// {
//     if (!poly.might_contain(x, y) ||
//         !pip(x, y, poly.ox, poly.oy)) return false;

//     for (size_t i = 0; i < poly.ix.size(); ++i)
//         if (poly.might_not_contain(x, y, i) &&
//             pip(x, y, poly.ix[i], poly.iy[i]))
//             return false;

//     return true;
// }

void ver2(const std::string &fpt,
          const std::string &fpoly,
          const std::string &o)
{
    typedef CGAL::Range_tree_map_traits_2<K, std::pair<int,int> > Traits;
    typedef CGAL::Range_tree_2<Traits> Range_tree_2_type;

    typedef Traits::Key Key;
    typedef Traits::Interval Interval;

    std::vector<Key> pts;
    std::vector<Polygon> polys;
    std::vector<std::pair<int, int> > infos;

    tbb::parallel_invoke([&] {

            std::string xmlstr;
            GMLParser gnp;
            char ch;
            int id, seq;

            std::ifstream fin_poly(fpoly);
            while (fin_poly >> ch) {
                Polygon poly;

                while (':' != ch) fin_poly >> ch;
                fin_poly >> id >> ch >> seq >> ch;
                infos.push_back(std::make_pair(id, seq));

                getline(fin_poly, xmlstr);
                gnp.polygon(xmlstr.c_str(),
                            poly.outer_ring, poly.inner_rings);

                polys.push_back(poly);
            }
            fin_poly.close();

        },
        [&] {

            std::string xmlstr;
            GMLParser gnp;
            char ch;
            double x, y;
            int id, seq;

            std::ifstream fin_point(fpt);
            while (fin_point >> ch) {
                while (':' != ch) fin_point >> ch;
                fin_point >> id >> ch >> seq >> ch;
                getline(fin_point, xmlstr);
                gnp.point(xmlstr.c_str(), x, y);
                pts.push_back(Key(K::Point_2(x, y),
                                  std::make_pair(id, seq)));
            }
            fin_point.close();

        });

    Range_tree_2_type tree(pts.begin(), pts.end());
    tbb::concurrent_vector<Result> results;

    for (size_t i = 0; i < polys.size(); ++i) {

        Polygon &p = polys[i];
        Ring &outer = p.outer_ring;
        Interval win(Interval(K::Point_2(outer.xa, outer.xb),
                              K::Point_2(outer.ya, outer.yb)));

        std::vector<Key> res;
        tree.window_query(win, std::back_inserter(res));

        for (size_t j = 0; j < res.size(); ++j) {

            Key &k = res[j];
            if (k.second.second > infos[i].second &&
                CGAL::ON_BOUNDED_SIDE ==
                CGAL::bounded_side_2(p.outer_ring.ring.begin(),
                                     p.outer_ring.ring.end(),
                                     k.first, K())) {

                bool f = true;
                for (size_t m = 0; m < p.inner_rings.size(); ++m) {
                    Ring &inner = p.inner_rings[m];
                    if (CGAL::ON_BOUNDED_SIDE ==
                        CGAL::bounded_side_2(inner.ring.begin(),
                                             inner.ring.end(),
                                             k.first, K())) {
                        f = false;
                        break;
                    }
                }

                if (f) {
                    ID a = std::make_pair(k.second.first,
                                          k.second.second);
                    ID b = std::make_pair(infos[i].first,
                                          infos[i].second);
                    results.push_back(std::make_pair(a, b));
                }
            }
        }
    }

    std::ofstream fo(o);
    for (size_t i = 0; i < results.size(); ++i) {
        Result &r = results[i];
        fo << r.first.first << ':' << r.first.second << ':'
           << r.second.first << ':' << r.second.second << std::endl;
    }
    fo.close();
}
