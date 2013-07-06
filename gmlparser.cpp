#include "gmlparser.h"

namespace SigSpatial2013 {

    GMLParser::GMLParser(void)
    {
        ax_initialize((void*)malloc, (void*)free);

        ax_initializeClassParser(&_classContext);
        _pointClass = ax_classFromString(PointSchema, &_classContext);
        _polyClass = ax_classFromString(PolygonSchema, &_classContext);

        ax_initializeParser(&_context, ChunkSize);
    }

    GMLParser::~GMLParser(void)
    {
        ax_releaseParser(&_context);
        ax_releaseClassParser(&_classContext);
    }

    bool GMLParser::point(const char *s, double &x, double &y)
    {
        AXElement *_gmlpoint = ax_parse(&_context, s, _pointClass, 1);
        AXAttribute *attr = &ax_getElement(_gmlpoint, 0)->attributes[0];

        char *p = const_cast<char *>(attr->begin);
        x = strtod(p, &p);
        y = strtod(++p, &p);

        return !_context.errorCode;
    }

    bool GMLParser::polygon(const char *s, Polygon &poly)
    {
        std::vector<Ring> &outer_rings = poly.outer_rings;
        std::vector<Ring> &inner_rings = poly.inner_rings;

        AXElement *_gmlpoly = ax_parse(&_context, s, _polyClass, 1);

        AXAttribute *attr =
            &ax_getElement(ax_getElement(ax_getElement(_gmlpoly, 0), 0), 0)->attributes[0];

        char *p = const_cast<char *>(attr->begin);
        char *e = const_cast<char *>(attr->limit);
        trim_right(e);

        Ring rings;

        rings.xa = rings.xb = strtod(p, &p);
        rings.ya = rings.yb = strtod(++p, &p);

        int beg = 0;

        while (p != e) {
            double x = strtod(p, &p);
            double y = strtod(++p, &p);

            if (x < rings.xa) rings.xa = x;
            if (x > rings.xb) rings.xb = x;
            if (y < rings.ya) rings.ya = y;
            if (y > rings.yb) rings.yb = y, beg = rings.ring.size();

            rings.ring.push_back(Point_2(x, y));
        }

        poly.xa = rings.xa;
        poly.xb = rings.xb;
        poly.ya = rings.ya;
        poly.yb = rings.yb;

        int sz = rings.ring.size();

        double bounds[4] = {rings.yb, rings.xa, rings.ya, rings.xb};
        double coords[2];

        for (int i = beg, f = 0; i < beg+sz;) {

            int ind = i % sz;

            Ring ring;

            ring.ring.push_back(rings.ring[ind]);

            int offset = 0;
            bool turn = false;

            for (int j = (ind+1) % sz; true; j = (j+1) % sz) {

                Point_2 &p = rings.ring[j];
                ring.ring.push_back(p);

                coords[(f+1)%2] = p.x();
                coords[f%2] = p.y();

                if (fuzzy_eq(coords[0], bounds[f])) {
                    turn = fuzzy_eq(coords[1], bounds[(f+1)%4]);
                    break;
                } else if (fuzzy_eq(coords[1], bounds[(f+1)%4])) {
                    offset = 1;
                    if (1 == f%2) ring.ring.push_back(Point_2(bounds[f], bounds[(f+1)%4]));
                    else ring.ring.push_back(Point_2(bounds[(f+1)%4], bounds[f]));
                    turn = true;
                    break;
                }
            }

            if (turn) ++f;
            if (ring.ring.size() > 2) outer_rings.push_back(ring);
            i += ring.ring.size() - 1 - offset;
        }

        AXElement *_rings = ax_getElement(_gmlpoly, 1);
        if (!_rings) return !_context.errorCode;

        _rings = _rings->firstChild;

        while (_rings) {
            AXAttribute * attr = &ax_getElement(_rings, 0)->attributes[0];
            char *p = const_cast<char *>(attr->begin);
            char *e = const_cast<char *>(attr->limit);
            trim_right(e);

            Ring ring;

            ring.xa = ring.xb = strtod(p, &p);
            ring.ya = ring.yb = strtod(++p, &p);

            while (p != e) {
                double x = strtod(p, &p);
                double y = strtod(++p, &p);

                if (x < ring.xa) ring.xa = x;
                if (x > ring.xb) ring.xb = x;
                if (y < ring.ya) ring.ya = y;
                if (y > ring.yb) ring.yb = y;

                ring.ring.push_back(Point_2(x, y));
            }

            inner_rings.push_back(ring);

            _rings = _rings->nextSibling;
        }

        return !_context.errorCode;
    }

}
