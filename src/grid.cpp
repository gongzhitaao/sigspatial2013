#include <algorithm>
#include "grid.h"

namespace SigSpatial2013 {

    void Grid::draw(double x0, double y0, double x1, double y1)
    {
        A_ = y1 >= y0 ? y1 - y0 : y0 - y1;
        B_ = x1 - x0;
        C_ = x1 * y0 - x0 * y1;

        if (abs(A_) <= abs(B_))
            _draw_1(floor(x0), floor(y0), floor(x1), floor(y1),
                    x1 >= x0 ? 1 : -1, y1 >= y0 ? 1 : -1);
        else
            _draw_2(floor(x0), floor(y0), floor(x1), floor(y1),
                    x1 >= x0 ? 1 : -1, y1 >= y0 ? 1 : -1);
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
