/*!
  \file   within.cpp
  \author Zhitao Gong <me@gongzhitaao.org>
  \date   Mon Jun  3 17:53:10 2013

  \brief  Code for WITHIN_n predicate checking.
*/
#include <fstream>
#include <string>
#include <vector>

#include <ppl.h>
#include <concurrent_vector.h>

#include "polygon.h"
#include "utils.h"

namespace SigSpatial2013 {

    /*! \brief Functor for WITHIN_n predicate checking
     */
    struct within_n
    {
        /*! \brief range tree constructed on
          SigSpatial2013::POINT_SIZE point
        */
        range_tree_t &t_;

        /*! \brief polygons with the same id but different sequence
          number
        */
        std::vector<PolygonSeq> &v_;

        concurrency::concurrent_vector<result_t> &r_; //!< result

        double n_;              //!< distance

        /*! \brief Constructor.  Preserve some states.
         */
        within_n(range_tree_t &t,
                 std::vector<PolygonSeq> &v,
                 concurrency::concurrent_vector<result_t> &r,
                 double n)
            : t_(t), v_(v), r_(r), n_(n) { }

        void operator () (size_t i) const
        {
            // Use squared distance thus avoiding the expensive sqrt
            double d2 = n_ * n_;

            PolygonSeq &ps = v_[i];

            // Loop throught all polygons with the same id but
            // different sequence number
            for (size_t j = 0; j < ps.seq_.size(); ++j) {

                Polygon &p = ps.v_[j];

                std::vector<node_t> res;
                t_.window_query(p.mbr(n_), std::back_inserter(res));

                for (size_t k = 0; k < res.size(); ++k) {

                    node_t &nd = res[k];

                    if (ps.most_recent_polygon(nd.second.second) != j)
                        continue;

                    if (p.within_n(nd.first, d2)) {
                        id_t a = std::make_pair(nd.second.first, nd.second.second);
                        id_t b = std::make_pair(i+1, ps.seq_[j]);
                        r_.push_back(std::make_pair(a, b));
                    }
                }
            }
        }
    };
}

void within(double n,
            const std::string &fpt,
            const std::string &fpoly,
            const std::string &o)
{
    using namespace SigSpatial2013;

    std::vector<PolygonSeq> polys;

    read_polygon(fpoly, polys, true);

    std::ofstream(o, std::ios::out);
    std::ofstream fo(o, std::ios::app);

    std::ifstream fin_point(fpt);
    for (bool b = true; b; ) {

        std::vector<node_t> v;
        b = read_point(fin_point, v);

        range_tree_t tree(v.begin(), v.end());

        concurrency::concurrent_vector<result_t> results;
        concurrency::parallel_for((size_t)0, polys.size(), (size_t)1, within_n(tree, polys, results, n));

        for (size_t i = 0; i < results.size(); ++i) {
            result_t &r = results[i];
            fo << r.first.first << ':' << r.first.second << ':'
               << r.second.first << ':' << r.second.second << std::endl;
        }

    }
    fin_point.close();
    fo.close();
}
