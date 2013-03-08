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
using namespace std;

#include <asm-xml.h>

#include "Defs.h"

const char* asString(AXAttribute *attr)
{
    const char* start = attr->begin;
    const char* limit = attr->limit;
    size_t size = limit - start;
    memcpy(buffer, start, size);
    buffer[size] = 0;
    return buffer;
}

void fillStream(stringstream &ss, AXAttribute *attr)
{
    streambuf *buf = ss.rdbuf();
    buf->sputn(attr->begin, attr->limit - attr->begin);
}

/* Algorithm source:
* http://www.ecse.rpi.edu/Homepages/wrf/Research/Short_Notes/pnpoly.html
* I still don't quite understand the following code.  Though it works!
*/
inline bool pip(double x, double y, const Ring &r)
{
    int n = r.size();
    bool c = false;
    for (int i = 0, j = n-1; i < n; j = i++)
        if ( ((r[i].second > y) != (r[j].second > y)) &&
            (x < (r[j].first - r[i].first) * (y - r[i].second) / (r[j].second - r[i].second) + r[i].first) )
            c = !c;
    return c;
}

bool inside(const Point &pt, const Polygon &poly)
{
    if (!pip(pt.coord.first, pt.coord.second, poly.outer)) return false;
    for (int i = 0; i < poly.inner.size(); ++i)
        if (pip(pt.coord.first, pt.coord.second, poly.inner[i])) return false;
    return true;
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
        fillStream(ss, &ax_getElement(gmlpoint, 0)->attributes[0]);
        ss >> tmppt.coord.first >> ch >> tmppt.coord.second;
        ss.str("");
        ss.clear();
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
        fillStream(ss, &ax_getElement(ax_getElement(ax_getElement(gmlpoly, 0), 0), 0)->attributes[0]);
        while (ss >> x >> ch >> y) tmppoly.outer.push_back(make_pair(x, y));
        ss.str("");
        ss.clear();
        /* Read in inner rings, i.e., the holes, if any.
        */
        rings = ax_getElement(gmlpoly, 1);
        if (rings) {
            ring = rings->firstChild;
            while (ring) {
                fillStream(ss, &ax_getElement(ring, 0)->attributes[0]);
                while (ss >> x >> ch >> y) tmpr.push_back(make_pair(x, y));
                ss.str("");
                ss.clear();
                tmppoly.inner.push_back(tmpr);
                tmpr.clear();
                ring = ring->nextSibling;
            }
        }
        polys.push_back(tmppoly);        
    }

    for (int i = 0; i < points.size(); ++i) {
        for (int j = 0; j < polys.size(); ++j) {
            if (points[i].seq < polys[j].seq) break;
            if (inside(points[i], polys[j]))
                fout << points[i].id << ':' << points[i].seq << "::"
                << polys[j].id << ':' << polys[j].seq << endl;
        }
    }

    fin_point.close();
    fin_poly.close();
    fout.close();
    ax_releaseParser(&context);
    ax_releaseClassParser(&classContext);
    return 0;
}