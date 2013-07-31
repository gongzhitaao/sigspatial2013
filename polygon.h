/*!
  \file   polygon.h
  \author Zhitao Gong <me@gongzhitaao.org>
  \date   Mon Jul  8 20:02:06 2013

  \brief  Polygon definition
*/

#ifndef _SIGSPATIAL2013_POLYGON_H_
#define _SIGSPATIAL2013_POLYGON_H_

#pragma once

#include "defs.h"

namespace SigSpatial2013 {

    class Ring
    {
    public:
        bool contains(const point_t &p) const;
        virtual bool within_n(const point_t &p, double d2) const { return false; }

        size_t size() const;
        void push(double x, double y);
        void mbr(double xa, double ya, double xb, double yb);

        double xa() const;
        double xb() const;
        double ya() const;
        double yb() const;

        point_t &operator[] (size_t i);
        const point_t &operator[] (size_t i) const;

    protected:
        std::vector<point_t> v_;
        double x_[2], y_[2];
    };

    class OuterRing : public Ring
    {
    public:
        OuterRing();

        bool within_n(const point_t &p, double d2) const;
        void corner();

    private:
        int corner_;
    };

    class InnerRing : public Ring
    {
    public:
        bool within_n(const point_t &p, double d2) const;
    };

    class Polygon
    {
    public:
        typedef std::pair<double, int> index_t;

        bool contains(const point_t &p) const;
        bool within_n(const point_t &p, double d2) const;

        void push(const Ring &v, int beg, bool within);
        void push(const InnerRing &r);

        interval_t mbr() const;
        void mbr(double xa, double ya, double xb, double yb);
        interval_t mbr(double d) const;

    private:
        void _sort(int f);
        bool _in_mbr(const point_t &p) const;
        bool _within_n(const point_t &p, double d2, int f) const;
        int _index(double d, int f) const;

    private:
        std::vector<OuterRing> outer_;
        std::vector<InnerRing> inner_;

        std::vector<index_t> b_[4];
        point_t corner_[4];
        double mbr_[4];
    };

    struct PolygonSeq
    {
        std::vector<int> seq_;
        std::vector<Polygon> v_;

        size_t most_recent_polygon(int seq);
    };
}

#include "polygon.hpp"

#endif /* _SIGSPATIAL2013_POLYGON_H_ */
