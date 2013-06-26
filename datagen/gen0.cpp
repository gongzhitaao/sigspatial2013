/*!
  \file   gen0.cpp
  \author Zhitao Gong <me@gongzhitaao.org>
  \date   Sat Jun 22 10:28:45 2013

  \brief 1M points and 500 polygons with 4k vertices each.  And all
  the points are inside the polygons.
*/
#include <cmath>

#include <vector>
#include <algorithm>
#include <fstream>

#include <boost/lexical_cast.hpp>

#include <CGAL/Simple_cartesian.h>
#include <CGAL/point_generators_2.h>
#include <CGAL/algorithm.h>
#include <CGAL/random_selection.h>

typedef CGAL::Simple_cartesian<double> R;
typedef R::Point_2 Point;
typedef CGAL::Creator_uniform_2<double,Point> Creator;

const double RADIUS = 100.0;
const int size = 4000;

int main()
{

    const char a[] = ":<gml:Polygon srsName=\"EPSG:54004\" xmlns:gml=\"http://www.opengis.net/gml\"><gml:outerBoundaryIs><gml:LinearRing><gml:coordinates decimal=\".\" cs=\",\" ts=\" \">";
    const char b[] = "</gml:coordinates></gml:LinearRing></gml:outerBoundaryIs></gml:Polygon>";

    const char c[] = ":<gml:Point srsName=\"EPSG:54004\" xmlns:gml=\"http://www.opengis.net/gml\"><gml:coordinates decimal=\".\" cs=\",\" ts=\" \">";
    const char d[] = "</gml:coordinates></gml:Point>";

    std::vector<std::pair<double, double> > v;
    v.reserve(size);
    double step = 2 * M_PI / size;

    std::ofstream o("poly500.in");
    for (int i = 0; i < 500; ++i) {
        o << "POLYGON:" << i+1 << ":" << i+1 << a;
        for (int j = 0; j < size; ++j)
            o << boost::lexical_cast<std::string>((RADIUS + i) * cos(step * j)) << ','
              << boost::lexical_cast<std::string>((RADIUS + i) * sin(step * j)) << ' ';
        o << b << std::endl;

    }
    o.clear();
    o.close();

    std::vector<Point> points;
    points.reserve(1000000);

    // Create 600 points within a disc of radius 150.
    CGAL::Random_points_in_disc_2<Point,Creator> g(150.0);
    CGAL::cpp11::copy_n( g, 1000000, std::back_inserter(points));

    o.open("point1000000.in");
    for (int i = 0; i < 1000000; ++i) {
        o << "POINT:" << i+1 << ":" << i+501 << c;
        o << boost::lexical_cast<std::string>(points[i].x()) << ','
          << boost::lexical_cast<std::string>(points[i].y()) << d << std::endl;
    }
    o.close();

    return 0;
}
