// file: Generator.cpp

#include "Generator.h"

using namespace std;

Generator::Generator(void)
    : _ptbeg("\
<gml:Point srsName=\"EPSG:54004\" xmlns:gml=\"http://www.opengis.net/gml\">\
  <gml:coordinates decimal=\".\" cs=\",\" ts=\" \">")
    , _ptend("\
  </gml:coordinates>\
</gml:Point>")
    , _polybeg("\
<gml:Polygon srsName=\"EPSG:54004\" xmlns:gml=\"http://www.opengis.net/gml\">")
    , _obeg("\
  <gml:outerBoundaryIs>\
    <gml:LinearRing>\
      <gml:coordinates decimal=\".\" cs=\",\" ts=\" \">")
    /*x1,y1 x2,y2 x3,y3 ... */
    , _poly1("\
      </gml:coordinates>\
    </gml:LinearRing>\
    </gml:outerBoundaryIs>")
    , _poly2("\
  <gml:innerBoundaryIs>\
    <gml:LinearRing>\
      <gml:coordinates decimal=\".\" cs=\",\" ts=\" \">")
    , _poly3("\
      </gml:coordinates>\
    </gml:LinearRing>\
  </gml:innerBoundaryIs>\
</gml:Polygon>")
{
}


Generator::~Generator(void)
{
}

string Generator::make_gml_point(string &s, double x, double y)
{
    return s = _ptbeg + to_string(x) + ',' + to_string(y) + _ptend;
}
