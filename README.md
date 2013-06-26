ACM SIGSPATIAL Cup 2013 Code
============================

# Problem Defintion

**Input**: A set of possibly overlapping regions (polygons with and
without interior rings) and a set of points.

**Output**: Pair each input point with one or more of the input polygons
such that a given spatial condition is satisfied between the polygon
and the point.

**Description**: A region can be any polygon (with or without interior
rings) that is made up of straight line segments.  A set of these
regions that can possibly overlap with each other are provided as part
of the input.  The number of the regions will be no more than a 500.
Additionally, a set of input points are given. The number of these
points will be no more than 1 Million.  The objective is to match each
of these points with one or more of the polygons from the input based
on a spatial predicate.  We have two spatial predicates, explained as
follows:

- INSIDE: which evaluates to TRUE if a point is inside the polygon.
  Notice that the point can be associated with one or more polygons
  (i.e., two overlapping polygons can contain the same point).  A
  point can be INSIDE any polygon with a sequence number (i.e.,
  timestamp) less than the sequence number of the point where only the
  latest position of each polygon (up to that sequence number) is
  considered.

- WITHIN n (e.g., WITHIN a distance of 1000 units): which evaluates to
  true if the point is at less than 1000 units distance from the
  polygon.  Notice that each point may be associated with one or more
  of the polygons, as the same point can be within 1000 units distance
  from several polygons.


for details, refer to this year's
[official page](http://dmlab.cs.umn.edu/GISCUP2013/problem.php)
