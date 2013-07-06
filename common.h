/*!
  \file   common.h
  \author Zhitao Gong <me@gongzhitaao.org>
  \date   Mon Jun  3 14:08:35 2013

  \brief  Common declarations used internally.
*/

#ifndef _SIG_COMMON_H_
#define _SIG_COMMON_H_

#pragma once

#include <vector>

// CGAL for range tree
#include <CGAL/Cartesian.h>
#include <CGAL/Polygon_2_algorithms.h>
#include <CGAL/Range_segment_tree_traits.h>
#include <CGAL/Range_tree_k.h>

namespace SigSpatial2013 {

    typedef CGAL::Cartesian<double> K; //!< CGAL Kernel
    typedef K::Point_2 Point_2;        //!< CGAL 2d point

    /*! \brief Associative data contains in each tree node, i.e., id
      and sequence number
    */
    typedef CGAL::Range_tree_map_traits_2<K, std::pair<int,int> > Traits;
    typedef CGAL::Range_tree_2<Traits> Range_tree_2_type; //!< Range tree type

    typedef Traits::Key Key;    //!< Tree node structure
    typedef Traits::Interval Interval; //!< Query window type

    /*! \brief Contains vertices of simple polygon without holes and
      it's bounding MBR.
    */
    struct Ring
    {
        std::vector<Point_2> ring; //!< Boundary vertices
        double xa, xb, ya, yb;     //!< Minimum bounding box (MBR)
    };

    /*! \brief Polygons with or without holes.
     */
    struct Polygon
    {
        std::vector<Ring> outer_rings; //!< Outer boundary
        std::vector<Ring> inner_rings; //!< Boundary for each holes
        double xa, xb, ya, yb;
    };

    /*! \brief Polygons with the same id but different sequence numbers.

      \details We don't store id here because the specification
      assumes that the ID of the polygons are consecutive and
      ascending natural numbers starting at 1.
    */
    struct PolygonSeq
    {
        std::vector<int> seq;   //!< Sequence number
        std::vector<Polygon> polys; //!< Corresponding polygon
    };

    /*! \brief ID here means neither point id nor polygon id, but the
      pair <id,sequence> that uniquely identifies a point or a polygon
      which will be appear in the output.
    */
    typedef std::pair<int, int> ID;
    /*! \brief All point and polygon pairs that satisfy a certain
      predicate, INSIDE or WITHIN_n.  All results will finally be
      output to a file in the format:
      `POINT_ID:POINT_SEQ:POLYGON_ID:POLYGON_SEQ`.
    */
    typedef std::pair<ID, ID> Result;

    /*! \brief Find the sequence number of the most recent polygon
      w.r.t a point.

      \details According the the spec, the most recent polygon w.r.t a
      point is the polygon with maximum sequence number smaller than
      the point's sequence number.

      \param ps Polygon Sequence
      \param seq sequence number of the point
      \return sequence number of the most recent polygon w.r.t #seq
    */
    std::size_t most_recent_polygon(const PolygonSeq &ps, int seq);

    /*! \brief Read from polygon file into vectors of PolygonSeq
      structure

      \param fpoly Polygon definition file
      \param v Vectors of PolygonSeq
    */
    void read_polygon(const std::string &fpoly,
                      std::vector<PolygonSeq> &v);

    /*! \brief The number of points read in at once.

      \details Because of the memory constraints, it's impratical to
      read in all the points at once.  And also, it's faster not to do
      so.  This is an empirical value.  Results show that this might
      be the right value.  According to my result, any values that are
      neither too small nor too big will do.  The time complexity of
      the program is approximately \f$ O(n\log n)\f$, So smaller
      values are preferred.  But too small values will incur too much
      overhead offsetting the benefit of smaller values.
    */
    const size_t POINT_SIZE = 1e4;

    /*! \brief Read from point file into vector of SigSpatial2013::Key
      used to construct the range tree.
    */
    bool read_point(std::ifstream &f, std::vector<Key> &v);

    /*! \brief According to the spec, "The resolution of the distance
      is two digits after decimal.".
    */
    const double PRECISION = 1e-2;

    /*! \brief a == b if abs(a-b) < PRECISION
    */
    inline bool fuzzy_eq(const double &a, const double &b)
    { return (-PRECISION < a-b) && (a-b < PRECISION); }

}

#endif /* _SIG_COMMON_H_ */
