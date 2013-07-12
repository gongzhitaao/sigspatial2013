/*!
  \file   defs.h
  \author Zhitao Gong <me@gongzhitaao.org>
  \date   Mon Jul  8 19:18:18 2013

  \brief  Common declarations used internally.
*/

#ifndef _SIG_DEFS_H_
#define _SIG_DEFS_H_

#pragma once

#include <vector>

// CGAL for range tree
#include <CGAL/Cartesian.h>
#include <CGAL/Polygon_2_algorithms.h>
#include <CGAL/Range_segment_tree_traits.h>
#include <CGAL/Range_tree_k.h>

namespace SigSpatial2013 {

    typedef CGAL::Cartesian<double> K;
    typedef K::Point_2 point_t;
    typedef K::Segment_2 segment_t;

    typedef CGAL::Range_tree_map_traits_2<K, std::pair<int,int> > Traits;
    typedef CGAL::Range_tree_2<Traits> range_tree_t;

    typedef Traits::Key node_t;
    typedef Traits::Interval interval_t;

    typedef std::pair<int, int> id_t;

    typedef std::pair<id_t, id_t> result_t;
}

#endif /* _SIG_DEFS_H_ */
