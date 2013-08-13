#include <cstring>

namespace SigSpatial2013 {

    inline void Grid::bound(double xa, double xb, double ya, double yb)
    {
        xa_ = xa, ya_ = ya;;
        deltax_ = (xb - xa) / SZ;
        deltay_ = (yb - ya) / SZ;
        memset(grid_, CGAL::ON_UNBOUNDED_SIDE, sizeof grid_);
    }

    inline int Grid::_x(double x) const
    {
        return (int)floor((x - xa_) / deltax_);
    }

    inline int Grid::_y(double y) const
    {
        return (int)floor((y - ya_) / deltay_);
    }

    inline void Grid::_draw_1(int x0, int y0, int x1, int y1, int sy)
    {
        for (int x = x0, y = y0; x != x1; ++x) {
            grid_[y][x] = CGAL::ON_BOUNDARY;
            arr_[y].push_back(x);

            switch (line_.oriented_side(point_t((x+1) * deltax_,
                                                (y+sy) * deltay_))) {
            case CGAL::ON_NEGATIVE_SIDE:
                y += sy;
                grid_[y][x] = CGAL::ON_BOUNDARY;
                arr_[y].push_back(x);
                break;
            case CGAL::ON_ORIENTED_BOUNDARY:
                y += sy;
                break;
            default: /* empty */;
            }
        }
    }

    inline void Grid::_draw_2(int x0, int y0, int x1, int y1, int sy)
    {
        for (int y = y0, x = x0; y != y1; y += sy) {
            grid_[y][x] = CGAL::ON_BOUNDARY;
            arr_[y].push_back(x);

            switch (line_.oriented_side(point_t((x+1) * deltax_,
                                                (y+sy) * deltay_))) {
            case CGAL::ON_POSITIVE_SIDE:
                ++x;
                grid_[y][x] = CGAL::ON_BOUNDARY;
                arr_[y].push_back(x);
                break;
            case CGAL::ON_ORIENTED_BOUNDARY:
                ++x;
                break;
            default: /* empty */;
            }
        }
    }

    inline CGAL::Bounded_side Grid::side(const point_t &p) const
    {
        return grid_[_y(p.y())][_x(p.x())];
    }
}
