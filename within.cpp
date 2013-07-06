/*!
  \file   within.cpp
  \author Zhitao Gong <me@gongzhitaao.org>
  \date   Mon Jun  3 17:53:10 2013

  \brief  Code for WITHIN_n predicate checking.
*/
#include "common.h"

#include <fstream>
#include <string>
#include <vector>

#include <tbb/concurrent_vector.h>
#include <tbb/parallel_for.h>

namespace SigSpatial2013 {

    /*! \brief Functor for WITHIN_n predicate checking
     */
    struct within_n
    {
        /*! \brief range tree constructed on
            SigSpatial2013::POINT_SIZE point
        */
        Range_tree_2_type &t_;

        /*! \brief polygons with the same id but different sequence
          number
        */
        std::vector<PolygonSeq> &v_;

        tbb::concurrent_vector<Result> &r_; //!< result

        double n_;              //!< distance

        /*! \brief Constructor.  Preserve some states.
         */
        within_n(Range_tree_2_type &t,
                 std::vector<PolygonSeq> &v,
                 tbb::concurrent_vector<Result> &r,
                 double n)
            : t_(t), v_(v), r_(r), n_(n) { }

        /*! \brief Return true when the point #p is within n units of
          distance of the polygon represented by its bounding
          vertices.

          \details Loop through all the vertices, return true
          immediately once a distance is smaller than #d2.  Note that
          the ponit could be inside or outside the polygon.  This
          function only determines wethers it's within n units
          distance of the boundary.

          \param p point to test
          \param v vectors of the boundary vertices
          \param d2 squared distance
          \return true if point #p is within within_n::n_ units distance of boundary.
        */
        bool within_n_of_boundary(const Point_2 &p,
                                  const std::vector<Point_2> &v,
                                  double d2) const
        {

            for (size_t i = 1; i < v.size(); ++i) {
                if (CGAL::squared_distance(p, K::Segment_2(v[i-1], v[i])) < d2) {
                    return true;
                }
            }

            return CGAL::squared_distance(p, K::Segment_2(v[0], v.back())) < d2;

        }

        void operator () (size_t i) const
        {/*
            // Use squared distance thus avoiding the expensive sqrt
            double d2 = n_ * n_;

            PolygonSeq &ps = v_[i];

            // Loop throught all polygons with the same id but
            // different sequence number
            for (size_t j = 0; j < ps.seq.size(); ++j) {

                Polygon &p = ps.polys[j];
                Ring &outer = p.outer_ring;

                // Construct a query window.  The query window here is
                // just the mbr of the polygon extended by n units
                // plus a small margin.  The margin, MARGIN, here is
                // required to include those points on the boundary
                // of the query windown.
                Interval win(Interval(K::Point_2(outer.xa-n_, outer.xb+n_),
                                      K::Point_2(outer.ya-n_, outer.yb+n_)));

                // The actual query is handled by the CGAL.
                std::vector<Key> res;
                t_.window_query(win, std::back_inserter(res));

                // Sift through every possible results and filter out
                // those unqualified.
                for (size_t k = 0; k < res.size(); ++k) {

                    Key &key = res[k];

                    // According to the spec, only the most recent
                    // polygon is considered, i.e., polygon with the
                    // maximum sequence number less than the point's
                    // sequence number
                    if (most_recent_polygon(ps, key.second.second) != j)
                        continue;

                    Point_2 &pt = key.first;

                    bool f = false;

                    if (outer.xa < pt.x() && pt.x() < outer.xb &&
                        outer.ya < pt.y() && pt.y() < outer.yb) {

                        CGAL::Bounded_side bs = CGAL::bounded_side_2(outer.ring.begin(), outer.ring.end(), pt, K());

                        switch (bs) {

                        case CGAL::ON_BOUNDED_SIDE:
                            {
                                f = true;

                                for (size_t m = 0; m < p.inner_rings.size(); ++m) {

                                    Ring &inner = p.inner_rings[m];

                                    CGAL::Bounded_side bs = CGAL::bounded_side_2(inner.ring.begin(), inner.ring.end(), pt, K());

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
                        ID a = std::make_pair(key.second.first, key.second.second);
                        ID b = std::make_pair(i+1, ps.seq[j]);
                        r_.push_back(std::make_pair(a, b));
                    }
                }
            }
         */}
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
        tbb::parallel_for((size_t)0, polys.size(), (size_t)1, within_n(tree, polys, results, n));

        for (size_t i = 0; i < results.size(); ++i) {
            Result &r = results[i];
            fo << r.first.first << ':' << r.first.second << ':'
               << r.second.first << ':' << r.second.second << std::endl;
        }

    }
    fin_point.close();
    fo.close();
}
