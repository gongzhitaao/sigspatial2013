// Version2.cpp

#include "core.h"

// standard headers
#include <algorithm>
#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <vector>
#include <utility>

// tbb for multi-processors
#include <tbb/parallel_invoke.h>
#include <tbb/concurrent_vector.h>
#include <tbb/parallel_for.h>

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

int most_recent_polygon(const PolygonSeq &ps, int seq)
{
    int b = 0, m = 0, e = ps.seq.size() - 1;

    if (seq >= ps.seq[e]) return e;

    while (b < e) {
        m = (b+e)/2;
        if (ps.seq[m] > seq) e = m;
        else b = m + 1;
    }

    return b-1;
}

void inside(const std::string &fpt,
            const std::string &fpoly,
            const std::string &o)
{
    typedef CGAL::Range_tree_map_traits_2<K, std::pair<int,int> > Traits;
    typedef CGAL::Range_tree_2<Traits> Range_tree_2_type;

    typedef Traits::Key Key;
    typedef Traits::Interval Interval;

    std::vector<Key> pts;
    std::vector<PolygonSeq> polys;

    tbb::parallel_invoke([&] {

            /* read in polygon */

            std::string xmlstr;
            GMLParser gnp;
            char ch;
            int id, seq;

            std::ifstream fin_poly(fpoly);
            while (fin_poly >> ch) {
                Polygon poly;

                while (':' != ch) fin_poly >> ch;
                fin_poly >> id >> ch >> seq >> ch;

                getline(fin_poly, xmlstr);
                gnp.polygon(xmlstr.c_str(),
                            poly.outer_ring, poly.inner_rings);

                if (id > polys.size()) {
                    PolygonSeq ps;
                    ps.seq.push_back(seq);
                    ps.polys.push_back(poly);
                    polys.push_back(ps);
                } else {
                    PolygonSeq &ps = polys[id-1];
                    ps.seq.push_back(seq);
                    ps.polys.push_back(poly);
                }

            }
            fin_poly.close();

        },
        [&] {

            /* read in point */

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

    // for each polygon id
    tbb::parallel_for((size_t)0, polys.size(), (size_t)1, [&] (size_t i) {

            // for (size_t i = 0; i < polys.size(); ++i) {

            PolygonSeq &ps = polys[i];

            // for each polygon sequence of the same id
            for (size_t j = 0; j < ps.seq.size(); ++j) {

                Polygon &p = ps.polys[j];
                Ring &outer = p.outer_ring;
                Interval win(Interval(K::Point_2(outer.xa, outer.xb),
                                      K::Point_2(outer.ya, outer.yb)));

                std::vector<Key> res;
                tree.window_query(win, std::back_inserter(res));

                for (size_t k = 0; k < res.size(); ++k) {

                    Key &key = res[k];

                    if (most_recent_polygon(ps, key.second.second) != j)
                        continue;

                    if (CGAL::ON_BOUNDED_SIDE ==
                        CGAL::bounded_side_2(p.outer_ring.ring.begin(),
                                             p.outer_ring.ring.end(),
                                             key.first, K())) {

                        bool f = true;
                        for (size_t m = 0; m < p.inner_rings.size(); ++m) {
                            Ring &inner = p.inner_rings[m];
                            if (!(CGAL::ON_UNBOUNDED_SIDE ==
                                  CGAL::bounded_side_2(inner.ring.begin(),
                                                       inner.ring.end(),
                                                       key.first, K()))) {
                                f = false;
                                break;
                            }
                        }

                        if (f) {
                            ID a = std::make_pair(key.second.first,
                                                  key.second.second);
                            ID b = std::make_pair(i+1, ps.seq[j]);
                            results.push_back(std::make_pair(a, b));
                        }
                    }
                }
            } // for each polygon seq
        }); // for each polygon id

    std::ofstream fo(o);
    for (size_t i = 0; i < results.size(); ++i) {
        Result &r = results[i];
        fo << r.first.first << ':' << r.first.second << ':'
           << r.second.first << ':' << r.second.second << std::endl;
    }
    fo.close();
}
