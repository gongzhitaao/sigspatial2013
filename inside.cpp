/*!
  \file   inside.cpp
  \author Zhitao Gong <me@gongzhitaao.org>
  \date   Mon Jun  3 17:52:43 2013

  \brief  Code for INSIDE predicate checking.
*/
#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <utility>

#include <tbb/concurrent_vector.h>
#include <tbb/parallel_for.h>

#include "common.h"
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

namespace SigSpatial2013 {

    /*! \brief Functor for point in polygon (pip) detection.
     */
    struct pip
    {
        Range_tree_2_type &t_;  //!< range tree containing point to test
        std::vector<PolygonSeq> &v_; //!< polygons to test against
        /*! \brief Contains all point and polygon pairs that satifies
          the INSIDE predicate.
        */
        tbb::concurrent_vector<Result> &r_;

        /*! \brief Constructor.

          \param t range tree
          \param v polygons
          \param r result
        */
        pip(Range_tree_2_type &t,
            std::vector<PolygonSeq> &v,
            tbb::concurrent_vector<Result> &r)
            : t_(t), v_(v), r_(r) { }

        void operator () (size_t i) const {

            PolygonSeq &ps = v_[i];

            // for each polygon sequence of the same id
            for (size_t j = 0; j < ps.seq.size(); ++j) {

                Polygon &p = ps.polys[j];
                Interval win(K::Point_2(p.xa, p.ya),
                             K::Point_2(p.xb, p.yb));

                std::vector<Key> res;
                t_.window_query(win, std::back_inserter(res));

                for (size_t k = 0; k < res.size(); ++k) {

                    Key &key = res[k];

                    if (most_recent_polygon(ps, key.second.second) != j)
                        continue;

                    bool outside = false;

                    for (size_t m = 0; m < p.outer_rings.size(); ++m) {

                        Ring &outer = p.outer_rings[m];

                        if (CGAL::ON_UNBOUNDED_SIDE !=
                            CGAL::bounded_side_2(outer.ring.begin(),
                                                 outer.ring.end(),
                                                 key.first, K())) {
                            outside = true;
                            break;
                        }
                    }

                    if (outside) continue;

                    for (size_t m = 0; m < p.inner_rings.size(); ++m) {

                        Ring &inner = p.inner_rings[m];

                        if (CGAL::ON_UNBOUNDED_SIDE !=
                            CGAL::bounded_side_2(inner.ring.begin(),
                                                 inner.ring.end(),
                                                 key.first, K())) {
                            outside = true;
                            break;
                        }
                    }

                    if (outside) continue;

                    ID a = std::make_pair(key.second.first,
                                          key.second.second);
                    ID b = std::make_pair(i+1, ps.seq[j]);
                    r_.push_back(std::make_pair(a, b));

                }
            }
        }
    };
}

void inside(const std::string &fpt,
            const std::string &fpoly,
            const std::string &o)
{
    using namespace SigSpatial2013;

    std::vector<PolygonSeq> polys;

    read_polygon(fpoly, polys);

    std::ofstream(o, std::ios::out);
    std::ofstream fo(o, std::ios::app);

    std::ifstream fin_point(fpt);
    for (bool b = true; b; ) {
        std::vector<Key> v;
        b = read_point(fin_point, v);

        Range_tree_2_type tree(v.begin(), v.end());

        tbb::concurrent_vector<Result> results;
        tbb::parallel_for((size_t)0, polys.size(), (size_t)1, pip(tree, polys, results));

        for (size_t i = 0; i < results.size(); ++i) {
            Result &r = results[i];
            fo << r.first.first << ':' << r.first.second << ':'
               << r.second.first << ':' << r.second.second << std::endl;
        }

    }
    fin_point.close();
    fo.close();
}
