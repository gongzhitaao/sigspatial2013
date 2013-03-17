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

bool GMLNaiveParser::point(const char *s, double &x, double &y)
{
    AXElement *_gmlpoint = ax_parse(&_context, s, _pointClass, 1);
    fill_stream(&ax_getElement(_gmlpoint, 0)->attributes[0]);
    _ss >> x >> _ch >> y;
    _ss.str("");
    _ss.clear();
    return !_context.errorCode;
}

bool GMLNaiveParser::polygon(const char *s,
                             vector<double> &ox, vector<double> &oy,
                             vector<vector<double> > &ix, vector<vector<double> > &iy)
{
    AXElement *_gmlpoly = ax_parse(&_context, s, _polyClass, 1);

    fill_stream(&ax_getElement(ax_getElement(ax_getElement(_gmlpoly, 0), 0), 0)->attributes[0]);
    while (_ss >> _x >> _ch >> _y) {
        ox.push_back(_x);
        oy.push_back(_y);
    }
    _ss.str("");
    _ss.clear();

    AXElement *_rings = ax_getElement(_gmlpoly, 1)->firstChild;
    while (_rings) {
        fill_stream(&ax_getElement(_rings, 0)->attributes[0]);
        while (_ss >> _x >> _ch >> _y) {
            _tmpx.push_back(_x);
            _tmpy.push_back(_y);
        }
        ix.push_back(_tmpx);
        iy.push_back(_tmpy);

        _ss.str("");
        _ss.clear();
        _tmpx.clear();
        _tmpy.clear();

        _rings = _rings->nextSibling;
    }

    return true;
}
