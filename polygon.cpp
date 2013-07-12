/*!
  \file   polygon.cpp
  \author Zhitao Gong <me@gongzhitaao.org>
  \date   Wed Jul 10 15:09:06 2013

  \brief  method
*/

#include "polygon.h"
#include "utils.h"

namespace SigSpatial2013 {

    bool Polygon::within_n(const point_t &p, double d2) const
    {
        if (_in_mbr(p)) {
            for (size_t i = 0; i < outer_.size(); ++i)
                if (outer_[i].contains(p))
                    return outer_[i].within_n(p, d2);

            for (size_t i = 0; i < inner_.size(); ++i)
                if (inner_[i].contains(p))
                    return inner_[i].within_n(p, d2);

            return true;
        } else {
            if (p.y() >= mbr_[0]) return _within_n(p, d2, 0);
            if (p.x() <= mbr_[1]) return _within_n(p, d2, 1);
            if (p.y() <= mbr_[2]) return _within_n(p, d2, 2);
            if (p.x() >= mbr_[3]) return _within_n(p, d2, 3);
        }
        return false;
    }

    void Polygon::push(const Ring &v, int beg, bool within)
    {
        int sz = v.size();
        double coords[2];

        mbr(v.xa(), v.ya(), v.xb(), v.yb());
        int f = 0;

        double xa, ya, xb, yb;

        for (int i = beg; i < beg + sz; ) {

            int ind = i % sz;
            OuterRing rng;

            xa = xb = v[ind].x();
            ya = yb = v[ind].y();

            rng.push(xa, ya);

            int offset = 0;
            bool turn = false;

            for (int j = (ind+1) % sz; true; j = (j+1) % sz) {
                const point_t &p = v[j];
                rng.push(p.x(), p.y());

                if (p.x() < xa) xa = p.x();
                if (p.x() > xb) xb = p.x();
                if (p.y() < ya) ya = p.y();
                if (p.y() > yb) yb = p.y();

                coords[(f+1)%2] = p.x();
                coords[f%2] = p.y();

                if (fuzzy_eq(coords[0], mbr_[f])) {
                    turn = fuzzy_eq(coords[1], mbr_[(f+1)%4]);
                    break;
                } else if (fuzzy_eq(coords[1], mbr_[(f+1)%4])) {
                    rng.push(corner_[f].x(), corner_[f].y());
                    offset = 1;
                    turn = true;
                    break;
                }
            }

            int bak = rng.size() - 1 - offset;

            if (within) {
                int t = (f + offset) % 4;
                int kk = (1 == offset || !turn) ? b_[t].size() : -f-1;
                if (0 == t % 2) b_[t].push_back(index_t(rng[bak].x(), kk));
                else b_[t].push_back(index_t(rng[bak].y(), kk));
            }

            if (turn) f = (f+1) % 4;
            i += bak;

            rng.mbr(xa, ya, xb, yb);
            outer_.push_back(rng);
        }

        _sort(f%4);
    }

    int Polygon::_index(double d, int f) const
    {
        size_t b = 0, m = 0, e = b_[f].size() - 1;

        if (f < 2) {
            // north, west
            if (d > b_[f][0].first) return b_[f][0].second;
            if (d < b_[f].back().first) return b_[(f+1)%4][0].second;

            while (b < e) {
                m = (b + e) / 2;
                if (b_[f][m].first < b) e = m;
                else b = m + 1;
            }

            --b;
        } else {
            // south, east
            if (d < b_[f][0].first) return b_[f][0].second;
            if (d > b_[f].back().first) return b_[(f+1)%4][0].second;

            while (b < e) {
                m = (b + e) / 2;
                if (b_[f][m].first > d) e = m;
                else b = m + 1;
            }
        }

        return b_[f][b].second;
    }

    size_t PolygonSeq::most_recent_polygon(int seq)
    {
        size_t b = 0, m = 0, e = seq_.size() - 1;

        if (seq >= seq_[e]) return e;

        while (b < e) {
            m = (b+e)/2;
            if (seq_[m] > seq) e = m;
            else b = m + 1;
        }

        return b-1;
    }
}
