/*!
  \file   polygon.cpp
  \author Zhitao Gong <me@gongzhitaao.org>
  \date   Wed Jul 10 15:09:06 2013

  \brief  method
*/

#include "polygon.h"

#include "gmlparser.h"

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

        // not reachable
        return true;
    }

    void Polygon::push(const Ring &v, int beg, bool within)
    {
        int sz = v.size();
        double coords[2];

        mbr(v.xa(), v.ya(), v.xb(), v.yb());
        grid_.bound(v.xa(), v.xb(), v.ya(), v.yb());

        int f = 0, offset = 0;
        bool turn = false;
        double xa, ya, xb, yb;

        double x0, y0;
        if (beg > 0) {
            x0 = v[beg-1].x();
            y0 = v[beg-1].y();
        } else {
            x0 = v[sz-1].x();
            y0 = v[sz-1].y();
        }

        for (int i = beg; i < beg + sz; ) {

            int ind = i % sz;
            OuterRing rng;

            xa = xb = v[ind].x();
            ya = yb = v[ind].y();

            rng.push(xa, ya);

            grid_.draw(x0, y0, xa, ya);
            x0 = xa, y0 = ya;

            if (turn && 0 == offset) {
                if (0 == f % 2) b_[f].push_back(index_t(xa, -f));
                else b_[f].push_back(index_t(ya, -f));
            }

            offset = 0;
            turn = false;

            for (int j = (ind+1) % sz; true; j = (j+1) % sz) {
                const point_t &p = v[j];
                rng.push(p.x(), p.y());

                grid_.draw(x0, y0, p.x(), p.y());
                x0 = p.x(), y0 = p.y();

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
                int kk = (1 == offset || !turn) ? outer_.size() : -f-1;

                if (0 == t % 2) b_[t].push_back(index_t(rng[bak].x(), kk));
                else b_[t].push_back(index_t(rng[bak].y(), kk));

                if (1 == offset) rng.corner();
            }

            if (turn) f = (f+1) % 4;
            i += bak;

            rng.mbr(xa, ya, xb, yb);
            outer_.push_back(rng);
        }

        grid_.fill();

        if (within) _sort(f%4);
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
                if (b_[f][m].first < d) e = m;
                else b = m + 1;
            }

        } else {
            // south, east
            if (d < b_[f][0].first) return b_[f][0].second;
            if (d > b_[f].back().first) return b_[(f+1)%4][0].second;

            while (b < e) {
                m = (b + e) / 2;
                if (b_[f][m].first > d) e = m;
                else b = m + 1;
            }
            --b;
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

    void read_polygon(const std::string &fpoly,
                      std::vector<PolygonSeq> &v,
                      bool within)
    {
        std::string xmlstr;
        GMLParser gp;
        char ch;
        size_t id, seq;

        std::ifstream fin_poly(fpoly);
        while (fin_poly >> ch) {
            Polygon poly;

            while (':' != ch) fin_poly >> ch;
            fin_poly >> id >> ch >> seq >> ch;

            getline(fin_poly, xmlstr);
            gp.polygon(xmlstr.c_str(), poly, within);

            if (id > v.size()) {
                PolygonSeq ps;
                ps.seq_.push_back(seq);
                ps.v_.push_back(poly);
                v.push_back(ps);
            } else {
                PolygonSeq &ps = v[id-1];
                ps.seq_.push_back(seq);
                ps.v_.push_back(poly);
            }
        }
        fin_poly.close();
    }

    bool read_point(std::ifstream &f, std::vector<node_t> &v)
    {
        std::string xmlstr;
        GMLParser gnp;
        char ch;
        size_t id, seq;
        double x, y;

        size_t i;
        for (i = 0; i < POINT_SIZE && f >> ch; ++i) {
            while (':' != ch) f >> ch;
            f >> id >> ch >> seq >> ch;
            getline(f, xmlstr);
            gnp.point(xmlstr.c_str(), x, y);
            v.push_back(node_t(point_t(x, y), std::make_pair(id, seq)));
        }

        return i == POINT_SIZE;
    }
}
