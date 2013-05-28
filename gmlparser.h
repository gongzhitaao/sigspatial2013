#ifndef _SIG_GMLPARSER_H_
#define _SIG_GMLPARSER_H_

#include <vector>

#include "asm-xml.h"

#include "common.h"

namespace SigSpatial2013 {

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

    class GMLParser
    {
    public:
        GMLParser(void);
        ~GMLParser(void);

        bool point(const char *s, double &x, double &y);
        bool polygon(const char *s,
                     Ring &outer_ring, std::vector<Ring> &inner_rings);

    private:
        void trim_right(char *&e)
        {
            --e;
            while (0 == (int)*e || isspace(*e)) --e;
            ++e;
        }

    private:
        enum { ChunkSize = 32 * 1024 * 1024 };

        AXParseContext _context;
        AXClassContext _classContext;
        AXElementClass *_pointClass;
        AXElementClass *_polyClass;
    };

}

#endif /* _SIG_GMLPARSER_H_ */
