#pragma once

#include <algorithm>
#include <vector>

#include "../common.h"

class btree
{
public:

    btree(void) { }

    ~btree(void) { }

    void insert(int id, int seq, double x, double y)
    {
        data_.push_back(Point(id, seq, x, y));
    }

    void build()
    {
        index_ = std::vector<int>(data_.size());
        tree_ = std::vector<node>(data_.size());

        for (size_t i = 0; i < index_.size(); ++i) index_[i] = i;

        binary_build(0, 0, data_.size(), 0);
    }

private:
    void binary_build(int idx, int a, int b, int axis)
    {
        if (a >= b) {
            tree_[idx].mbr[axis][0] = tree_[idx].mbr[axis][1]
            = data_[index_[a]].coord[axis];
            tree_[idx].mbr[1-axis][0] = tree_[idx].mbr[1-axis][1]
            = data_[index_[a]].coord[1-axis];
        }

        std::sort(index_.begin()+a, index_.begin()+b+1,
            [&] (const Point &a, const Point &b) {
                return a.coord[axis] < b.coord[axis];
        });

        tree_[idx].mbr[axis][0] = data_[index_[a]].coord[axis];
        tree_[idx].mbr[axis][1] = data_[index_[b]].coord[axis];
        find_minmax(a, b, axis,
            tree_[idx].mbr[1-axis][0], tree_[idx].mbr[1-axis][0]);

        int c = (a+b)%2 ? (a+b)/2 : (a+b-1);
        binary_build(2*idx+1, a, c, 1-axis);
        binary_build(2*idx+2, c+1, b, 1-axis);
    }

    void find_minmax(int a, int b, int axis, double &min, double &max)
    {
        int i = (b-a+1) % 2;

        if (i) {
            min = max = data_[index_[a]].coord[axis];
            if (data_[index_[a+1]].coord[axis] > max)
                max = data_[index_[a+1]].coord[axis];
            else if (data_[index_[a+1]].coord[axis] < min)
                min = data_[index_[a+1]].coord[axis];
        }

        for (; i <= b; i += 2) {
            if (data_[index_[i]].coord[axis] > data_[i+1].coord[axis]) {
                if (data_[index_[i]].coord[axis] > max)
                    max = data_[index_[i]].coord[axis];
                if (data_[index_[i+1]].coord[axis] < min)
                    min = data_[index_[i+1]].coord[axis];
            } else {
                if (data_[index_[i]].coord[axis] < min)
                    min = data_[index_[i]].coord[axis];
                if (data_[index_[i+1]].coord[axis] > max)
                    max = data_[index_[i+1]].coord[axis];
            }
        }
    }

    struct node
    {
        double mbr[2][2];
        int a, b;

        node()
        {
            mbr[0][0] = mbr[1][0] = std::numeric_limits<double>::max();
            mbr[0][1] = mbr[1][1] = std::numeric_limits<double>::max();
        }
    };

    std::vector<int> index_;
    std::vector<node> tree_;
    std::vector<Point> data_;
};

