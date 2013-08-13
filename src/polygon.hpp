/*!
  \file   polygon.hpp
  \author Zhitao Gong <me@gongzhitaao.org>
  \date   Mon Jul  8 20:02:06 2013

  \brief  inline methods
*/

namespace SigSpatial2013 {

    inline bool Ring::contains(const point_t &p) const
    {
        return x_[0] <= p.x() && p.x() <= x_[1] && y_[0] <= p.y() && p.y() <= y_[1] &&
            CGAL::ON_UNBOUNDED_SIDE != CGAL::bounded_side_2(v_.begin(), v_.end(), p, K());
    }

    inline void Ring::push(double x, double y)
    {
        v_.push_back(point_t(x, y));
    }

    inline size_t Ring::size() const
    {
        return v_.size();
    }

    inline void Ring::mbr(double xa, double ya, double xb, double yb)
    {
        x_[0] = xa, x_[1] = xb;
        y_[0] = ya, y_[1] = yb;
    }

    inline double Ring::xa() const
    {
        return x_[0];
    }

    inline double Ring::xb() const
    {
        return x_[1];
    }

    inline double Ring::ya() const
    {
        return y_[0];
    }

    inline double Ring::yb() const
    {
        return y_[1];
    }

    inline point_t &Ring::operator[] (size_t i)
    {
        return v_[i];
    }

    inline const point_t &Ring::operator[] (size_t i) const
    {
        return v_[i];
    }

    inline OuterRing::OuterRing()
        : Ring(), corner_(false)
    { }

    inline bool OuterRing::within_n(const point_t &p, double d2) const
    {
        size_t sz = v_.size() - corner_;
        for (size_t i = 1; i < sz; ++i) {
            if (CGAL::squared_distance(p, segment_t(v_[i-1], v_[i])) < d2)
                return true;
        }
        return false;
    }

    inline void OuterRing::corner()
    {
        corner_ = 1;
    }

    inline bool InnerRing::within_n(const point_t &p, double d2) const
    {
        for (size_t i = 1; i < size(); ++i) {
            if (CGAL::squared_distance(p, segment_t(v_[i-1], v_[i])) < d2)
                return true;
        }
        return CGAL::squared_distance(p, segment_t(v_[0], v_.back())) < d2;
    }

    inline bool Polygon::contains(const point_t &p) const
    {
        switch (grid_.side(p)) {
        case CGAL::ON_BOUNDED_SIDE: return true;
        case CGAL::ON_UNBOUNDED_SIDE: return false;
        default:
            for (size_t i = 0; i < outer_.size(); ++i)
                if (outer_[i].size() > 2 &&
                    outer_[i].contains(p)) return false;

            for (size_t i = 0; i < inner_.size(); ++i)
                if (inner_[i].contains(p)) return false;

            return true;
        }
    }

    inline bool Polygon::_within_n(const point_t &p, double d2, int f) const
    {
        double xy = (0 == f % 2) ? p.x() : p.y();
        int ind = _index(xy, f);

        if (ind < 0) return CGAL::squared_distance(p, corner_[-ind-1]) < d2;
        else return outer_[ind].within_n(p, d2);
    }

    inline void Polygon::push(const InnerRing &r)
    {
        inner_.push_back(r);
    }

    inline void Polygon::mbr(double xa, double ya, double xb, double yb)
    {
        mbr_[0] = yb, mbr_[1] = xa, mbr_[2] = ya, mbr_[3] = xb;
        corner_[0] = point_t(xa, yb);
        corner_[1] = point_t(xa, ya);
        corner_[2] = point_t(xb, ya);
        corner_[3] = point_t(xb, yb);
    }

    inline interval_t Polygon::mbr(double d) const
    {
        return interval_t(point_t(mbr_[1]-d, mbr_[2]-d),
                          point_t(mbr_[3]+d, mbr_[0]+d));
    }

    inline interval_t Polygon::mbr() const
    {
        return interval_t(point_t(mbr_[1], mbr_[2]),
                          point_t(mbr_[3], mbr_[0]));
    }

    inline void Polygon::_sort(int f)
    {
        if (f < 2)
            std::sort(b_[f].begin(), b_[f].end(),
                      [] (const index_t &a, const index_t &b) {
                          return a.first > b.first;
                      });
        else
            std::sort(b_[f].begin(), b_[f].end(),
                      [] (const index_t &a, const index_t &b) {
                          return a.first < b.first;
                      });
    }

    inline bool Polygon::_in_mbr(const point_t &p) const
    {
        return (mbr_[1] < p.x() && p.x() < mbr_[3] &&
                mbr_[2] < p.y() && p.y() < mbr_[0]);
    }
}
