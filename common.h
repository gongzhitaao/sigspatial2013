#ifndef _SIG_COMMON_H_
#define _SIG_COMMON_H_

#pragma once

#include <vector>

// cgal for range tree
#include <CGAL/Cartesian.h>
#include <CGAL/Polygon_2_algorithms.h>
#include <CGAL/Range_segment_tree_traits.h>
#include <CGAL/Range_tree_k.h>

namespace SigSpatial2013 {

    typedef CGAL::Cartesian<double> K;
    typedef K::Point_2 Point_2;

    typedef CGAL::Range_tree_map_traits_2<K, std::pair<int,int> > Traits;
    typedef CGAL::Range_tree_2<Traits> Range_tree_2_type;

    typedef Traits::Key Key;
    typedef Traits::Interval Interval;

    struct Ring
    {
        std::vector<Point_2> ring;
        double xa, xb, ya, yb;
    };

    struct Polygon
    {
        Ring outer_ring;
        std::vector<Ring> inner_rings;
    };

    struct PolygonSeq
    {
        std::vector<int> seq;
        std::vector<Polygon> polys;
    };

    typedef std::pair<int, int> ID;
    typedef std::pair<ID, ID> Result;

    std::size_t most_recent_polygon(const PolygonSeq &ps, int seq);

    void read_polygon(const std::string &fpoly,
                      std::vector<PolygonSeq> &v);

    const size_t POINT_SIZE = 1e4;
    bool read_point(std::ifstream &f, std::vector<Key> &v);

    const double MARGIN = 0.01;
}

#endif /* _SIG_COMMON_H_ */
