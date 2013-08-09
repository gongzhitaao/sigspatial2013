#include <cstring>

namespace SigSpatial2013 {

    inline void Grid::bound(double xa, double xb, double ya, double yb)
    {
        deltax_ = (xb - xa) / (SZ + 1);
        deltay_ = (yb - ya) / (SZ + 1);
        memset(grid_, CGAL::ON_UNBOUNDED_SIDE, sizeof grid_);;
    }

    inline double Grid::_f(double x, double y)
    {
        return A_ * x - B_ * y + C_;
    }

    inline void Grid::_draw_1(int x0, int y0, int x1, int y1,
                              int sx, int sy)
    {
        for (int x = x0, y = y0; x != x1; x += sy) {
            grid_[y][x] = CGAL::ON_BOUNDARY;
            arr_[y].push_back(x);
            double f = _f((x+1) * deltax_, (y+sy) * deltay_);
            if (fuzzy_eq(f, 0)) y += sy;
            else if (f * A_ * B_ > 0) {
                y += sy;
                grid_[y][x] = CGAL::ON_BOUNDARY;
                arr_[y].push_back(x);
            }
        }
        std::printf("helojfjf\n");
    }

    inline void Grid::_draw_2(int x0, int y0, int x1, int y1,
                              int sx, int sy)
    {
        for (int y = y0, x = x0; y != y1; y += sy) {
            grid_[y][x] = CGAL::ON_BOUNDARY;
            arr_[y].push_back(x);
            double f = _f((x+sx) * deltax_, (y+1) * deltay_);
            if (fuzzy_eq(f, 0)) x += sx;
            else if (f * A_ * B_ < 0) {
                x += sx;
                grid_[y][x] = CGAL::ON_BOUNDARY;
                arr_[y].push_back(x);
            }
        }
    }

    inline CGAL::Bounded_side Grid::side(const point_t &p) const
    {
        return grid_[(int) floor(p.y() / deltay_)][(int) floor(p.x() / deltax_)];
    }
}
