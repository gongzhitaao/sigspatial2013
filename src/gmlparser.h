/*!
  \file   gmlparser.h
  \author Zhitao Gong <me@gongzhitaao.org>
  \date   Mon Jun  3 14:09:08 2013

  \brief Simple GML parser based on
  [AsmXml](http://tibleiz.net/asm-xml/).
*/

#ifndef _SIG_GMLPARSER_H_
#define _SIG_GMLPARSER_H_

#include <vector>

#include "asm-xml.h"

#include "polygon.h"

namespace SigSpatial2013 {

    const char PointSchema[] = "\
<schema>\
  <document name=\"gml:Point\">\
    <element name=\"gml:coordinates\" type=\"text\">\
    </element>\
  </document>\
</schema>";

    const char PolygonSchema[] = "\
<schema>\
  <document name=\"gml:Polygon\">\
    <element name=\"gml:outerBoundaryIs\">\
      <element name=\"gml:LinearRing\">\
        <element name=\"gml:coordinates\" type=\"text\">\
        </element>\
      </element>\
    </element>\
    <collection name=\"gml:innerBoundaryIs\">\
      <element name=\"gml:LinearRing\">\
        <element name=\"gml:coordinates\" type=\"text\">\
        </element>\
      </element>\
    </collection>\
  </document>\
</schema>";

    class GMLParser
    {
    public:
        GMLParser(void);
        ~GMLParser(void);

        bool point(const char *s, double &x, double &y);
        bool polygon(const char *s, Polygon &poly, bool within);

    private:
        int _make_ring(Ring &r, const char *beg, const char *end);

        void trim_right(char *&e)
        {
            --e;
            while (0 == (int)*e || isspace(*e)) --e;
            ++e;
        }

    private:
        enum { ChunkSize = 64 * 1024 * 1024 };

        AXParseContext _context;
        AXClassContext _classContext;
        AXElementClass *_pointClass;
        AXElementClass *_polyClass;
    };

}

#endif /* _SIG_GMLPARSER_H_ */
