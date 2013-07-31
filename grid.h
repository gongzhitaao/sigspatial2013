#ifndef _GRID_H_
#define _GRID_H_

#pragma once

#include <vector>
#include <utility>

namespace SigSpatial2013 {

    class Grid
    {
    public:
        void set(int id, int seq);
        void push(double xa, double ya, double xb, double yb);
        void fill();

    private:
        enum { SZ = 100 };

        typedef std::pair<int, signed char> T1; // seq and flag
        typedef std::pair<int, T1> T2;          // id and <seq,flag>

        std::vector<T2> grid_[SZ][SZ];
    };

    extern Grid grid;
}

#endif /* _GRID_H_ */
