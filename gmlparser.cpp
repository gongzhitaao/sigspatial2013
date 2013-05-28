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

    bool GMLParser::polygon(const char *s,
                            Ring &outer_ring,
                            std::vector<Ring> &inner_rings)
    {
        AXElement *_gmlpoly = ax_parse(&_context, s, _polyClass, 1);

        AXAttribute *attr =
            &ax_getElement(ax_getElement(ax_getElement(_gmlpoly, 0), 0), 0)->attributes[0];

        char *p = const_cast<char *>(attr->begin);
        char *e = const_cast<char *>(attr->limit);
        trim_right(e);

        outer_ring.xa = outer_ring.xb = strtod(p, &p);
        outer_ring.ya = outer_ring.yb = strtod(++p, &p);

        while (p != e) {
            double x = strtod(p, &p);
            double y = strtod(++p, &p);

            if (x < outer_ring.xa) outer_ring.xa = x;
            if (x > outer_ring.xb) outer_ring.xb = x;
            if (y < outer_ring.ya) outer_ring.ya = y;
            if (y > outer_ring.yb) outer_ring.yb = y;

            outer_ring.ring.push_back(Point_2(x, y));
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
