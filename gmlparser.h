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

#include "common.h"

namespace SigSpatial2013 {

    //! \name AsmXml Variables
    //! Schemas used by AsmXml to parse the GML
    //!@{

    /*! \brief point schema with all fields and attributes ignored but
      coordinates.
    */
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

    /*! \brief polygon schema with all fields and attributes ignored
        but coordinates of inner and outer boundaries.
     */
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
    //!@}

    /*! \brief Simple
      [GML](http://www.opengeospatial.org/standards/gml) parser
      based on [AsmXml](http://tibleiz.net/asm-xml/).
    */
    class GMLParser
    {
    public:

        /*! \brief ctor.  Initialize the AsmXml parser here.
         */
        GMLParser(void);

        /*! \brief dtor.  Free resources of AsmXml parser.
         */
        ~GMLParser(void);

        /*! \brief Parse GML point syntax.

          \param[in] s a valid GML point string
          \param[out] x x coordinate
          \param[out] y y coordinate

          \return true if everything is OK.
        */
        bool point(const char *s, double &x, double &y);

        /*! \brief Parse GML polygon syntax

          \param[in] s a valid GML polygon string
          \param[out] outer_ring outer boundary of the polygon
          \param[out] inner_rings boundaries for holes inside the polygon

          \return true if everything is OK.
        */
        bool polygon(const char *s, Ring &outer_ring, std::vector<Ring> &inner_rings);

    private:
        /*! \brief Remove trailing spaces.

          \param e pointer to the last character in a string.
        */
        void trim_right(char *&e)
        {
            --e;
            while (0 == (int)*e || isspace(*e)) --e;
            ++e;
        }

    private:
        /*! \brief buffer size for AsmXml parser.
         */
        enum { ChunkSize = 64 * 1024 * 1024 };

        //! \name AsmXml Variables
        //! Internal variables used by AsmXml parser
        //!@{
        AXParseContext _context;
        AXClassContext _classContext;
        AXElementClass *_pointClass;
        AXElementClass *_polyClass;
        //!@}
    };

}

#endif /* _SIG_GMLPARSER_H_ */
