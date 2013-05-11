#ifndef _SIG_COMMON_H_
#define _SIG_COMMON_H_

#pragma once

#include <vector>

// cgal for range tree
#include <CGAL/Cartesian.h>

typedef CGAL::Cartesian<double> K;
typedef K::Point_2 Point_2;

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

#endif /* _SIG_COMMON_H_ */
