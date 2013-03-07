/* File: version1.cpp
 * Author: Zhitao Gong
 * Created: Mar 1, 2013
 *
 * This is a naive solution using brutal force, i.e., just compaire
 * every paire of points and polygons to find possible paires.
 * It's guaranteed that the program will be 100% correct.  And also
 * this program will give us a loose upper bound for the running time.
 */

#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <utility>
#include <vector>
using namespace std;

#include <asm-xml.h>

typedef pair<double, double> Coord;
typedef vector<Coord> Ring;

struct Point
{
	int id, seq;
	Coord coord;
};

struct Polygon
{
	int id, seq;
	Ring outer;
    vector<Ring> inner;
};

const int RAW_XML_SIZE = 8 * 1024 * 1024;

char xmlstr[RAW_XML_SIZE];
char buffer[RAW_XML_SIZE];

const char* asString(AXAttribute* attr)
{
  const char* start = attr->begin;
  const char* limit = attr->limit;
  size_t size = limit - start;
  memcpy(buffer, start, size);
  buffer[size] = 0;
  return buffer;
}

bool inside(const Point &pt, const Polygon &poly)
{
    if (!pip(pt.coord.first, pt.coord.second, poly.outer))
        return false;
    bool r = false;
    for (int i = 0; i < poly.inner.size(); ++i) {
    }
}

/* Algorithm source:
 * http://www.ecse.rpi.edu/Homepages/wrf/Research/Short_Notes/pnpoly.html
 * I still don't quite understand the following code.  Though it works!
 */
inline bool pip(double x, double y, const Ring &r)
{
    int i, j, n = r.size();
    bool c = false;
    for (int i = 0, j = n-1; i < n; j = i++)
        if ( ((r[i].second > y) != (r[j].second > y)) &&
            (x < (r[j].first - r[i].first) * (y - r[i].second) / (r[j].second - r[i].second) + r[i].first) )
            c = !c;
    return c;
}

int main(int argc, char *argv[])
{
	/* Parse input argv, input format:
     * Geofence < ”INSIDE”|”WITHIN n”> < PointInputFilePath > < PolygonInputFilePath > < OutputFilePath >
     */
	int i = 1;
	int n;
	if (!strncmp(argv[i], "\"INSIDE\"", 8)) {
	} else if (!strncmp(argv[i], "\"WITHIN", 6)) {
		n = atoi(argv[++i]);
        ++i; // skip `"'
	}
    string point_file = argv[++i];
	string polygon_file = argv[++i];
    string output_file = argv[++i];

    ifstream fin_point(point_file);
    ifstream fin_poly(polygon_file);
    stringstream ss(stringstream::in | stringstream::out);
    ofstream fout(output_file);

    vector<Point> points;
    vector<Polygon> polys;
    Point tmppt;
    Polygon tmppoly;
    Ring tmpr;
    char ch;
    double x, y;

    /* AsmXml variables
     */
    AXClassContext classContext;
    AXElementClass *pointClass, *polyClass;
    const char pointSchema[] = "\
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

    const char polygonSchema[] = "\
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

    AXParseContext context;
    /* 256 byte chunkSize is enough for point parsing.
     * But as points in a polygon may be as many as 200k, the estimated
     * chunkSize for the raw data may be around 16M.  So set the chunkSize
     * to 32M for safety.
     */
    unsigned chunkSize = 32 * 1024 * 1024;

    AXElement *gmlpoint, *gmlpoly, *rings, *ring;

    ax_initialize(malloc, free);

    ax_initializeClassParser(&classContext);
    pointClass = ax_classFromString(pointSchema, &classContext);
    polyClass = ax_classFromString(polygonSchema, &classContext);
    
    ax_initializeParser(&context, chunkSize);

    /* Read in points.  1G memory is enough for all the points.
     */
    while (fin_point >> ch) {
        /* 1. Skip the initial string, i.e., "POINT:"
         * 2. Get the non-spatial data, i.e., id and sequence.
         * 3. Get and parse the rest XML-format spatial information
         */
        while (':' != ch) fin_point >> ch;
        fin_point >> tmppt.id >> ch >> tmppt.seq >> ch;
        fin_point.getline(xmlstr, RAW_XML_SIZE);
        gmlpoint = ax_parse(&context, xmlstr, pointClass, 1);
        ss << asString(&ax_getElement(gmlpoint, 0)->attributes[0]);
        ss >> tmppt.coord.first >> ch >> tmppt.coord.second;
        points.push_back(tmppt);
    }

    /* Read in polygons.  1G is NOT enough for all the polygons.
     * For the first phase, just assume that it can.
     * TODO sequentially read in polygons.
     */
    while (fin_poly >> ch) {
        while (':' != ch) fin_poly >> ch;
        fin_poly >> tmppoly.id >> ch >> tmppoly.seq >> ch;
        fin_poly.getline(xmlstr, RAW_XML_SIZE);
        gmlpoly = ax_parse(&context, xmlstr, polyClass, 1);
        /* Read in outer ring, i.e., the outer boundary.
         */
        tmppoly.outer.clear();
        ss << asString(&ax_getElement(ax_getElement(ax_getElement(gmlpoly, 0), 0), 0)->attributes[0]);
        while (ss >> x >> ch >> y) tmppoly.outer.push_back(make_pair(x, y));
        /* Read in inner rings, i.e., the holes, if any.
         */
        rings = ax_getElement(gmlpoly, 0);
        if (rings) {
            ring = rings->firstChild;
            while (ring) {
                ss << asString(&ax_getElement(ring, 0)->attributes[0]);
                while (ss >> x >> ch >> y) tmpr.push_back(make_pair(x, y));
                tmppoly.inner.push_back(tmpr);
                tmpr.clear();
                ring = ring->nextSibling;
            }
        }
        polys.push_back(tmppoly);        
    }

    for (int i = 0; i < points.size(); ++i) {
        for (int j = 0; j < polys.size(); ++j) {

        }
    }

    fin_point.close();
    fin_poly.close();
    fout.close();
    ax_releaseParser(&context);
    ax_releaseClassParser(&classContext);
	return 0;
}