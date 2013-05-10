#include "gmlparser.h"

#include <limits>

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
    fill_stream(&ax_getElement(_gmlpoint, 0)->attributes[0]);
    _ss >> x >> _ch >> y;
    _ss.str("");
    _ss.clear();
    return !_context.errorCode;
}

bool GMLParser::polygon(const char *s,
                        Ring &outer_ring,
                        std::vector<Ring> &inner_rings)
{
    AXElement *_gmlpoly = ax_parse(&_context, s, _polyClass, 1);

    fill_stream(&ax_getElement(ax_getElement(ax_getElement(_gmlpoly, 0), 0), 0)->attributes[0]);

    outer_ring.xa = outer_ring.ya = std::numeric_limits<double>::max();
    outer_ring.xb = outer_ring.yb = std::numeric_limits<double>::min();
    while (_ss >> _x >> _ch >> _y) {
        if (_x < outer_ring.xa) outer_ring.xa = _x;
        if (_x > outer_ring.xb) outer_ring.xb = _x;
        if (_y < outer_ring.ya) outer_ring.ya = _y;
        if (_y > outer_ring.yb) outer_ring.yb = _y;
        outer_ring.ring.push_back(Point_2(_x, _y));
    }

    _ss.str("");
    _ss.clear();

    AXElement *_rings = ax_getElement(_gmlpoly, 1);
    if (!_rings) return true;

    _rings = _rings->firstChild;
    while (_rings) {
        fill_stream(&ax_getElement(_rings, 0)->attributes[0]);

        Ring ring;

        while (_ss >> _x >> _ch >> _y) {
            if (_x < ring.xa) ring.xa = _x;
            if (_x > ring.xb) ring.xb = _x;
            if (_y < ring.ya) ring.ya = _y;
            if (_y > ring.yb) ring.yb = _y;
            ring.ring.push_back(Point_2(_x, _y));
        }

        inner_rings.push_back(ring);

        _ss.str("");
        _ss.clear();

        _rings = _rings->nextSibling;
    }

    return true;
}
