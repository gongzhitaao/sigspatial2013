#ifndef _SIGSPATIAL2013_GRID_H_
#define _SIGSPATIAL2013_GRID_H_

#pragma once

#include "defs.h"

namespace SigSpatial2013 {

    class Grid
    {
    public:
        void bound(double xa, double xb, double ya, double yb);
        void draw(double x0, double y0, double x1, double y1);
        void fill();

        int side(const point_t &p) const;

    private:
        double _f(double x, double y) const;

        int _x(double x) const;
        int _y(double y) const;

        void _draw_1(int x0, int y0, int x1, int y1, int sy, int orient);
        void _draw_2(int x0, int y0, int x1, int y1, int sy, int orient);

    private:
        enum { SZ = 100 };

        double deltax_, deltay_;
        double xa_, ya_;
        line_t line_;

        int val_[SZ+1][SZ+1];
    };
}

#include "grid.hpp"

#endif /* _SIGSPATIAL2013_GRID_H_ */
