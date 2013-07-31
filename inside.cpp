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

#include "gmlparser.h"
#include "polygon.h"
#include "utils.h"

namespace SigSpatial2013 {

    struct pip
    {
        range_tree_t &t_;
        std::vector<PolygonSeq> &v_;
        tbb::concurrent_vector<result_t> &r_;

        pip(range_tree_t &t,
            std::vector<PolygonSeq> &v,
            tbb::concurrent_vector<result_t> &r)
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

        tbb::concurrent_vector<result_t> results;
        tbb::parallel_for((size_t)0, polys.size(), (size_t)1,
                          pip(tree, polys, results));

        for (size_t i = 0; i < results.size(); ++i) {
            result_t &r = results[i];
            fo << r.first.first << ':' << r.first.second
               << ':'
               << r.second.first << ':' << r.second.second
               << std::endl;
        }

    }
    fin_point.close();
    fo.close();
}
