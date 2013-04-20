#include "GMLNaiveParser.h"

#include <cstdlib>
using namespace std;

GMLNaiveParser::GMLNaiveParser(void)
{
    ax_initialize(malloc, free);

    ax_initializeClassParser(&_classContext);
    _pointClass = ax_classFromString(PointSchema, &_classContext);
    _polyClass = ax_classFromString(PolygonSchema, &_classContext);

    ax_initializeParser(&_context, ChunkSize);
}

GMLNaiveParser::~GMLNaiveParser(void)
{
    ax_releaseParser(&_context);
    ax_releaseClassParser(&_classContext);
}

bool GMLNaiveParser::point(const char *s, Point &pt)
{
    AXElement *_gmlpoint = ax_parse(&_context, s, _pointClass, 1);
    fill_stream(&ax_getElement(_gmlpoint, 0)->attributes[0]);
    _ss >> pt.x >> _ch >> pt.y;
    _ss.str("");
    _ss.clear();
    return !_context.errorCode;
}

bool GMLNaiveParser::polygon(const char *s, Polygon &poly)
{
    AXElement *_gmlpoly = ax_parse(&_context, s, _polyClass, 1);

    fill_stream(&ax_getElement(ax_getElement(ax_getElement(_gmlpoly, 0), 0), 0)->attributes[0]);
    poly.oxx[0] = poly.oyy[0] = numeric_limits<double>::max();
    poly.oxx[1] = poly.oyy[1] = numeric_limits<double>::min();
    while (_ss >> _x >> _ch >> _y) {

        if (_x < poly.oxx[0]) {
            if (poly.oxx[0] > poly.oxx[1]) poly.oxx[1] = poly.oxx[0];
            poly.oxx[0] = _x;
        } else if (_x > poly.oxx[1]) {
            if (poly.oxx[1] < poly.oxx[0]) poly.oxx[0] = poly.oxx[1];
            poly.oxx[1] = _x;
        }

        if (_y < poly.oyy[0]) {
            if (poly.oyy[0] > poly.oyy[1]) poly.oyy[1] = poly.oyy[0];
            poly.oyy[0] = _y;
        } else if (_y > poly.oyy[1]) {
            if (poly.oyy[1] < poly.oyy[0]) poly.oyy[0] = poly.oyy[1];
            poly.oyy[1] = _y;
        }

        poly.ox.push_back(_x);
        poly.oy.push_back(_y);

    }
    _ss.str("");
    _ss.clear();

    AXElement *_rings = ax_getElement(_gmlpoly, 1);
    if (!_rings) return true;

    _rings = _rings->firstChild;
    while (_rings) {
        fill_stream(&ax_getElement(_rings, 0)->attributes[0]);
        double xa = numeric_limits<double>::max(), xb = numeric_limits<double>::min();
        double ya = numeric_limits<double>::max(), yb = numeric_limits<double>::min();
        std::vector<double> _tmpx, _tmpy;
        while (_ss >> _x >> _ch >> _y) {
            _tmpx.push_back(_x);
            _tmpy.push_back(_y);

            if (_x < xa) {
                if (xa > xb) xb = xa;
                xa = _x;
            } else if (_x > xb) {
                if (xb < xa) xa = xb;
                xb = _x;
            }

            if (_y < ya) {
                if (ya > yb) yb = ya;
                ya = _y;
            } else if (_y > yb) {
                if (yb < ya) ya = yb;
                yb = _y;
            }
        }

        poly.ixa.push_back(xa);
        poly.ixb.push_back(xb);
        poly.iya.push_back(ya);
        poly.iyb.push_back(yb);
        poly.ix.push_back(_tmpx);
        poly.iy.push_back(_tmpy);

        _ss.str("");
        _ss.clear();

        _rings = _rings->nextSibling;
    }

    return true;
}
