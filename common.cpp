#include "common.h"

#include <fstream>
#include <string>
#include <vector>

#include "gmlparser.h"

namespace SigSpatial2013 {

    size_t most_recent_polygon(const PolygonSeq &ps, int seq)
    {
        size_t b = 0, m = 0, e = ps.seq.size() - 1;

        if (seq >= ps.seq[e]) return e;

        while (b < e) {
            m = (b+e)/2;
            if (ps.seq[m] > seq) e = m;
            else b = m + 1;
        }

        return b-1;
    }

    void read_polygon(const std::string &fpoly,
                      std::vector<PolygonSeq> &v)
    {
        std::string xmlstr;
        GMLParser gnp;
        char ch;
        size_t id, seq;

        std::ifstream fin_poly(fpoly);
        while (fin_poly >> ch) {
            Polygon poly;

            poly.ring.ring.clear();

            while (':' != ch) fin_poly >> ch;
            fin_poly >> id >> ch >> seq >> ch;

            getline(fin_poly, xmlstr);
            gnp.polygon(xmlstr.c_str(), poly);

            if (id > v.size()) {
                PolygonSeq ps;
                ps.seq.push_back(seq);
                ps.polys.push_back(poly);
                v.push_back(ps);
            } else {
                PolygonSeq &ps = v[id-1];
                ps.seq.push_back(seq);
                ps.polys.push_back(poly);
            }

        }
        fin_poly.close();
    }

    bool read_point(std::ifstream &f, std::vector<Key> &v)
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
            v.push_back(Key(K::Point_2(x, y),
                            std::make_pair(id, seq)));
        }

        return i == POINT_SIZE;
    }

}
