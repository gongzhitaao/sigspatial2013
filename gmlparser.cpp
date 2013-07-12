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
        AXElement *_gmlpoint = ax_parse(&_context, s, _pointClass, 0);
        AXAttribute *attr = &ax_getElement(_gmlpoint, 0)->attributes[0];

        char *p = const_cast<char *>(attr->begin);
        x = strtod(p, &p);
        y = strtod(++p, &p);

        return !_context.errorCode;
    }

    int GMLParser::_make_ring(Ring &r, const char *beg, const char *end)
    {
        char *p = const_cast<char *>(beg);
        char *e = const_cast<char *>(end);
        trim_right(e);

        double xa, xb, ya, yb;

        xa = xb = strtod(p, &p);
        ya = yb = strtod(++p, &p);

        int start = 0;

        while (p != e) {
            double x = strtod(p, &p);
            double y = strtod(++p, &p);

            if (x < xa) xa = x;
            if (x > xb) xb = x;
            if (y < ya) ya = y;
            if (y > yb) yb = y, start = r.size();

            r.push(x, y);
        }

        r.mbr(xa, ya, xb, yb);

        return start;
    }

    bool GMLParser::polygon(const char *s, Polygon &poly, bool within = false)
    {
        AXElement *_gmlpoly = ax_parse(&_context, s, _polyClass, 0);

        AXAttribute *attr =
            &ax_getElement(ax_getElement(ax_getElement(_gmlpoly, 0), 0), 0)->attributes[0];

        Ring r;
        int beg = _make_ring(r, attr->begin, attr->limit);

        poly.push(r, beg, within);

        AXElement *_inners = _gmlpoly->firstChild;

        while (_inners) {
            AXAttribute * attr = &ax_getElement(ax_getElement(_inners, 0), 0)->attributes[0];

            InnerRing r;
            _make_ring(r, attr->begin, attr->limit);
            poly.push(r);

            _inners = _inners->nextSibling;
        }

        return !_context.errorCode;
    }
}
