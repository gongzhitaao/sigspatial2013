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

class GMLNaiveParser
{
public:
    GMLNaiveParser(void);
    ~GMLNaiveParser(void);

    bool point(const char *s, double &x, double &y);
    bool polygon(const char *s,
                 std::vector<double> &ox, std::vector<double> &oy,
                 std::vector<std::vector<double> > &ix, std::vector<std::vector<double> > &iy);

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
    std::vector<double> _tmpx, _tmpy;
};
