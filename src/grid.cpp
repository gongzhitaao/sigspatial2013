#include <algorithm>
#include "grid.h"

namespace SigSpatial2013 {

    void Grid::draw(double x0, double y0, double x1, double y1)
    {
        double x[2], y[2];

        if (x1 >= x0) {
            x[0] = x0, y[0] = y0;
            x[1] = x1, y[1] = y1;
        } else {
            x[0] = x1, y[0] = y1;
            x[1] = x0, y[1] = y0;
        }

        int sy;
        double a, b, c;
        if (y[1] >= y[0]) {
            a = y[1] - y[0];
            b = x[1] - x[0];
            c = x[1] * y[0] - x[0] * y[1];
            sy = 1;
        } else {
            a = y[0] - y[1];
            b = x[0] - x[1];
            c = x[0] * y[1] - x[1] * y[0];
            sy = -1;
        }
        line_ = line_t(a, -b, c);

        if (a <= b)
            _draw_1(_x(x[0]), _y(y[0]), _x(x[1]), _y(y[1]), sy);
        else
            _draw_2(_x(x[0]), _y(y[0]), _x(x[1]), _y(y[1]), sy);
    }

    void Grid::fill()
    {
        for (int r = 0; r < SZ; ++r) {
            std::sort(arr_[r].begin(), arr_[r].end());
            for (size_t i = 1; i < arr_[r].size(); i += 2) {
                if (arr_[r][i] > arr_[r][i-1])
                    memset(&grid_[r][arr_[r][i]], CGAL::ON_BOUNDED_SIDE,
                           (arr_[r][i-1]-arr_[r][i]) * sizeof(CGAL::Bounded_side));
            }
        }
    }
}
