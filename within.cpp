// within.cpp

#include "common.h"

#include <fstream>
#include <string>
#include <vector>

#include <tbb/concurrent_vector.h>
#include <tbb/parallel_for.h>

namespace SigSpatial2013 {

    struct within_n
    {
        Range_tree_2_type &t_;
        std::vector<PolygonSeq> &v_;
        tbb::concurrent_vector<Result> &r_;

        double n_;

        within_n(Range_tree_2_type &t,
                 std::vector<PolygonSeq> &v,
                 tbb::concurrent_vector<Result> &r,
                 double n)
            : t_(t), v_(v), r_(r), n_(n) { }

        bool within_n_of_boundary(const Point_2 &p,
                                  const std::vector<Point_2> &v,
                                  double d2) const
        {

            for (size_t i = 1; i < v.size(); ++i) {
                if (CGAL::squared_distance(p,
                                           K::Segment_2(v[i-1], v[i])) < d2) {
                    return true;
                }
            }

            return
                CGAL::squared_distance(p,
                                       K::Segment_2(v[0], v.back())) < d2;

        }

        void operator () (size_t i) const
        {
            // Use squared distance, avoid the expensive sqrt
            double d2 = n_ * n_;

            PolygonSeq &ps = v_[i];

            for (size_t j = 0; j < ps.seq.size(); ++j) {

                Polygon &p = ps.polys[j];
                Ring &outer = p.outer_ring;

                Interval win(Interval(K::Point_2(outer.xa-n_-MARGIN, outer.xb+n_+MARGIN),
                                      K::Point_2(outer.ya-n_-MARGIN, outer.yb+n_+MARGIN)));

                std::vector<Key> res;
                t_.window_query(win, std::back_inserter(res));

                for (size_t k = 0; k < res.size(); ++k) {

                    Key &key = res[k];

                    if (most_recent_polygon(ps, key.second.second) != j)
                        continue;

                    Point_2 &pt = key.first;

                    bool f = false;

                    if (outer.xa < pt.x() && pt.x() < outer.xb &&
                        outer.ya < pt.y() && pt.y() < outer.yb) {

                        CGAL::Bounded_side bs =
                            CGAL::bounded_side_2(outer.ring.begin(),
                                                 outer.ring.end(),
                                                 pt, K());

                        switch (bs) {

                        case CGAL::ON_BOUNDED_SIDE:
                            {
                                f = true;

                                for (size_t m = 0; m < p.inner_rings.size(); ++m) {

                                    Ring &inner = p.inner_rings[m];

                                    CGAL::Bounded_side bs =
                                        CGAL::bounded_side_2(inner.ring.begin(),
                                                             inner.ring.end(),
                                                             pt, K());

                                    bool br = false;

                                    switch (bs) {

                                    case CGAL::ON_BOUNDED_SIDE:

                                        f = within_n_of_boundary(pt, inner.ring, d2);
                                        br = true;

                                        break;

                                    case CGAL::ON_BOUNDARY:

                                        br = true;
                                        break;

                                    default:

                                        break;
                                    }

                                    if (br) break;

                                }

                            }

                            break;

                        case CGAL::ON_UNBOUNDED_SIDE:

                            f = within_n_of_boundary(pt, outer.ring, d2);
                            break;

                        default:

                            f = true;
                            break;

                        }

                    } else {

                        f = within_n_of_boundary(pt, outer.ring, d2);
                    }

                    if (f) {
                        ID a = std::make_pair(key.second.first,
                                              key.second.second);
                        ID b = std::make_pair(i+1, ps.seq[j]);
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

    read_polygon(fpoly, polys);

    std::ofstream(o, std::ios::out);
    std::ofstream fo(o, std::ios::app);

    std::ifstream fin_point(fpt);
    for (bool b = true; b; ) {

        std::vector<Key> v;
        b = read_point(fin_point, v);

        Range_tree_2_type tree(v.begin(), v.end());

        tbb::concurrent_vector<Result> results;
        tbb::parallel_for((size_t)0, polys.size(), (size_t)1,
                          within_n(tree, polys, results, n));

        for (size_t i = 0; i < results.size(); ++i) {
            Result &r = results[i];
            fo << r.first.first << ':' << r.first.second << ':'
               << r.second.first << ':' << r.second.second << std::endl;
        }

    }
    fin_point.close();
    fo.close();
}
