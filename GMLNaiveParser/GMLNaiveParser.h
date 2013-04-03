#pragma once

#include <sstream>
#include <vector>

#include <asm-xml.h>

const char PointSchema[] = "\
<schema>\
  <document name=\"gml:Point\">\
    <attribute name=\"srsName\" ignore=\"yes\"/>\
    <attribute name=\"xmlns:gml\" ignore=\"yes\"/>\
    <element name=\"gml:coordinates\" type=\"text\">\
      <attribute name=\"decimal\" ignore=\"yes\"/>\
      <attribute name=\"cs\" ignore=\"yes\"/>\
      <attribute name=\"ts\" ignore=\"yes\"/>\
    </element>\
  </document>\
</schema>";

const char PolygonSchema[] = "\
<schema>\
  <document name=\"gml:Polygon\">\
    <attribute name=\"srsName\" ignore=\"yes\"/>\
    <attribute name=\"xmlns:gml\" ignore=\"yes\"/>\
    <element name=\"gml:outerBoundaryIs\">\
      <element name=\"gml:LinearRing\">\
        <element name=\"gml:coordinates\" type=\"text\">\
          <attribute name=\"decimal\" ignore=\"yes\"/>\
          <attribute name=\"cs\" ignore=\"yes\"/>\
          <attribute name=\"ts\" ignore=\"yes\"/>\
        </element>\
      </element>\
    </element>\
    <element name=\"gml:innerBoundaryIs\">\
      <collection name=\"gml:LinearRing\">\
        <element name=\"gml:coordinates\" type=\"text\">\
          <attribute name=\"decimal\" ignore=\"yes\"/>\
          <attribute name=\"cs\" ignore=\"yes\"/>\
          <attribute name=\"ts\" ignore=\"yes\"/>\
        </element>\
      </collection>\
    </element>\
  </document>\
</schema>";

struct Points
{
    std::vector<double> x, y;
    std::vector<int> id, seq;
};

struct Polygon
{
    std::vector<double> ox, oy;
    std::vector<std::vector<double> > ix, iy;

    double oxx[2], oyy[2];
    std::vector<double> ixa, ixb, iya, iyb;

    int id, seq;

    bool might_contain(double x, double y) {
        return oxx[0] < x && x < oxx[1] && oyy[0] < y && y < oyy[1];
    }

    bool might_not_contain(double x, double y, int i) {
        return ixa[i] < x && x < ixb[i] && iya[i] < y && y < iyb[i];
    }
};

class GMLNaiveParser
{
public:
    GMLNaiveParser(void);
    ~GMLNaiveParser(void);

    bool point(const char *s, double &x, double &y);
    bool polygon(const char *s, Polygon &poly);

private:
    inline void fill_stream(AXAttribute *attr)
    {  _ss.rdbuf()->sputn(attr->begin, attr->limit - attr->begin); }

private:
    enum { ChunkSize = 32 * 1024 * 1024 };

    AXParseContext _context;
    AXClassContext _classContext;
    AXElementClass *_pointClass;
    AXElementClass *_polyClass;

    std::stringstream _ss;
 
    char _ch;
    double _x, _y;
};
