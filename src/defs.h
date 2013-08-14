/*!
  \file   defs.h
  \author Zhitao Gong <me@gongzhitaao.org>
  \date   Mon Jul  8 19:18:18 2013

  \brief  Common declarations used internally.
*/

#ifndef _SIGSPATIAL2013_DEFS_H_
#define _SIGSPATIAL2013_DEFS_H_

#pragma once

#include <utility>

#include <CGAL/enum.h>
#include <CGAL/Cartesian.h>
#include <CGAL/Polygon_2_algorithms.h>
#include <CGAL/Range_segment_tree_traits.h>
#include <CGAL/Range_tree_k.h>

namespace SigSpatial2013 {

    typedef CGAL::Cartesian<double> K;
    typedef K::Point_2 point_t;
    typedef K::Segment_2 segment_t;
    typedef K::Line_2 line_t;

    typedef CGAL::Range_tree_map_traits_2<K, std::pair<int,int> > Traits;
    typedef CGAL::Range_tree_2<Traits> range_tree_t;

    typedef Traits::Key node_t;
    typedef Traits::Interval interval_t;

    typedef std::pair<int, int> id_t;

    typedef std::pair<id_t, id_t> result_t;

    const double PRECISION = 1e-2;

    inline bool fuzzy_eq(const double &a, const double &b)
    { return (-PRECISION < a-b) && (a-b < PRECISION); }
}

#endif /* _SIGSPATIAL2013_DEFS_H_ */
