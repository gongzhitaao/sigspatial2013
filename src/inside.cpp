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

#include <ppl.h>
#include <concurrent_vector.h>

#include "gmlparser.h"
#include "polygon.h"
#include "utils.h"

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
        range_tree_t &t_;  //!< range tree containing point to test
        std::vector<PolygonSeq> &v_; //!< polygons to test against
        /*! \brief Contains all point and polygon pairs that satifies
          the INSIDE predicate.
        */
        concurrency::concurrent_vector<result_t> &r_;

        /*! \brief Constructor.

          \param t range tree
          \param v polygons
          \param r result
        */
        pip(range_tree_t &t,
            std::vector<PolygonSeq> &v,
            concurrency::concurrent_vector<result_t> &r)
            : t_(t), v_(v), r_(r) { }

        void operator () (size_t i) const {

            PolygonSeq &ps = v_[i];

            for (size_t j = 0; j < ps.seq_.size(); ++j) {

                Polygon &p = ps.v_[j];

                std::vector<node_t> res;
                t_.window_query(p.mbr(), std::back_inserter(res));

                for (size_t k = 0; k < res.size(); ++k) {
                    node_t &nd = res[k];

                    if (ps.most_recent_polygon(nd.second.second) != j)
                        continue;

                    if (p.contains(nd.first)) {
                        id_t a = std::make_pair(nd.second.first,
                                                nd.second.second);
                        id_t b = std::make_pair(i+1, ps.seq_[j]);
                        r_.push_back(std::make_pair(a, b));
                    }
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

        std::vector<node_t> v;
        b = read_point(fin_point, v);

        range_tree_t tree(v.begin(), v.end());

        concurrency::concurrent_vector<result_t> results;
        concurrency::parallel_for((size_t)0, polys.size(), (size_t)1, pip(tree, polys, results));

        for (size_t i = 0; i < results.size(); ++i) {
            result_t &r = results[i];
            fo << r.first.first << ':' << r.first.second << ':'
               << r.second.first << ':' << r.second.second << std::endl;
        }

    }
    fin_point.close();
    fo.close();
}
