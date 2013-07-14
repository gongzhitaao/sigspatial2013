
#include "utils.h"

#include <fstream>
#include <string>
#include <vector>

#include "gmlparser.h"

namespace SigSpatial2013 {

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
