//! commom.h Common definitions used internally.

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

    typedef CGAL::Range_tree_map_traits_2<K, std::pair<int,int> > Traits; //!< Associative data contains in each tree node, i.e., ID and sequence number
    typedef CGAL::Range_tree_2<Traits> Range_tree_2_type; //!< Range tree type

    typedef Traits::Key Key;    //!< Tree node structure
    typedef Traits::Interval Interval; //!< Query window type

    /*! \brief Contains vertices of simple polygon without holes and
     *         it's bounding MBR.
     */
    struct Ring
    {
        std::vector<Point_2> ring; //!< Boundary vertices
        double xa,                 //!< min value along X axis
            xb,                    //!< max value along X axis
            ya,                    //!< min value along Y axis
            yb;                    //!< max value along Y axis
    };

    /*! \brief Polygon with or without holes.
     */
    struct Polygon
    {
        Ring outer_ring;        //!< Outer boundary
        std::vector<Ring> inner_rings; //!< Boundary for each holes
    };

    /*! \brief Polygons with the same ID but different sequence number.
     *
     *  \details We don't store ID here because the specification
     *  assumes that the ID of the polygons are consecutive and
     *  ascending natural numbers starting at 1.
     */
    struct PolygonSeq
    {
        std::vector<int> seq;   //!< Sequence number
        std::vector<Polygon> polys; //!< Corresponding polygon
    };

    typedef std::pair<int, int> ID; //!< The pair <ID,SEQ> identifies a point or a polygon
    typedef std::pair<ID, ID> Result; //!< Result output to a file

    /*! \brief Find the sequence number of the most recent polygon
     *         w.r.t a point.
     *
     *  \details According the the spec, the most recent polygon w.r.t
     *  a point is the polygon with maximum sequence number smaller
     *  than the point's sequence number.
     *
     *  \param ps Polygon Sequence
     *  \param seq Sequence number of the point
     *  \return Sequence number of the most recent polygon w.r.t #seq
     */
    std::size_t most_recent_polygon(const PolygonSeq &ps, int seq);

    /*! \brief Read from polygons files into vectors of PolygonSeq
     *         structure
     *
     *  \param fpoly Polygon definition file
     *  \param v Vectors of PolygonSeq
     */
    void read_polygon(const std::string &fpoly,
                      std::vector<PolygonSeq> &v);

    const size_t POINT_SIZE = 1e4; /*!< The number of points read in
                                     at once.  Because of the memory
                                     constraints, it's impratical to
                                     read in all the points at once.
                                     And also, it's faster not to do
                                     so.
                                   */
    /*! \brief Read from point file into vectors of Key
     */
    bool read_point(std::ifstream &f, std::vector<Key> &v);

    const double MARGIN = 0.01; /*!< Extend the query window by MARGIN
                                   in the WITHIN_N test to include the
                                   points on the boundary.  In the
                                   spec, the precision is two digits
                                   after decimal.
                                 */
}

#endif /* _SIG_COMMON_H_ */
