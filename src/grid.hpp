#include <cstring>

namespace SigSpatial2013 {

    inline void Grid::bound(double xa, double xb, double ya, double yb)
    {
        xa_ = xa, ya_ = ya;;
        deltax_ = (xb - xa) / SZ;
        deltay_ = (yb - ya) / SZ;
        memset(val_, 2, sizeof val_);
    }

    inline int Grid::_x(double x) const
    {
        return (int)floor((x - xa_) / deltax_);
    }

    inline int Grid::_y(double y) const
    {
        return (int)floor((y - ya_) / deltay_);
    }

    inline void Grid::_draw_1(int x0, int y0, int x1, int y1,
                              int sy, int orient)
    {
        for (int x = x0, y = y0; x != x1; ++x) {
            if (2 == val_[y][x]) val_[y][x] = 0;

            if (CGAL::ON_POSITIVE_SIDE !=
                line_.oriented_side(point_t((x+1) * deltax_,
                                            (y+sy) * deltay_))) {
                val_[y][x] += orient;
                y += sy;

                if (2 == val_[y][x]) val_[y][x] = 0;
                val_[y][x] += orient;
            }
        }
    }

    inline void Grid::_draw_2(int x0, int y0, int x1, int y1,
                              int sy, int orient)
    {
        for (int y = y0, x = x0; y != y1; y += sy) {

            if (2 == val_[y][x]) val_[y][x] = 0;
            val_[y][x] += orient;

            if (CGAL::ON_NEGATIVE_SIDE !=
                line_.oriented_side(point_t((x+1) * deltax_,
                                            (y+sy) * deltay_))) {
                ++x;
            }
        }
    }

    inline int Grid::side(const point_t &p) const
    {
        return val_[_y(p.y())][_x(p.x())];
    }
}
