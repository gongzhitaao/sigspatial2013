#include <fstream>
#include <iostream>
#include <string>
#include <utility>
#include <vector>
using namespace std;

#include <asm-xml.h>

typedef pair<double, double> coord_type;

struct Point
{
	int id, seq;
	coord_type coord;
};

struct Polygon
{
	int id, seq;
	vector<coord_type> outer, inner;
};

char buffer[256];
const char* asString(AXAttribute* attr)
{
  const char* start = attr->begin;
  const char* limit = attr->limit;
  size_t size = limit - start;
  memcpy(buffer, start, size);
  buffer[size] = 0;
  return buffer;
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

	/* Load input point and polygon file.
     * Because the number of points will be less than 1e6, we could load them into memory.
     * But not so lucky for polygons.
     */
    ifstream fin_point(point_file);
    ofstream fout(output_file);

    vector<Point> points;
    Point tmppt;
    char ch, xmlstr[256];

    /* AsmXml variables
     */
    AXClassContext classContext;
    AXElementClass *pointClass;
    const char pointSchema[] = "\
<schema>\
  <document name=\"gml:Point\">\
    <attribute name=\"srsName\" ignore=\"yes\"/>\
    <attribute name=\"xmlns:gml\" ignore=\"yes\"/>\
    <collection name=\"gml:coordinates\" type=\"text\">\
      <attribute name=\"decimal\" ignore=\"yes\"/>\
      <attribute name=\"cs\" ignore=\"yes\"/>\
      <attribute name=\"ts\" ignore=\"yes\"/>\
    </collection>\
  </document>\
</schema>";

    AXParseContext context;
    unsigned chunkSize = 256;

    AXElement *gmlpoint;

    ax_initialize(malloc, free);

    ax_initializeClassParser(&classContext);
    pointClass = ax_classFromString(pointSchema, &classContext);
    
    ax_initializeParser(&context, chunkSize);

    while (fin_point >> ch) {
        // skip the initial string, i.e. "POINT:"
        while (':' != ch) fin_point >> ch;
        fin_point >> tmppt.id >> ch >> tmppt.seq >> ch;
        fin_point.getline(xmlstr, 256);
        gmlpoint = ax_parse(&context, xmlstr, pointClass, 1);
        fout << asString(&gmlpoint->firstChild->attributes[0]) << endl;
    }
    fin_point.close();
    fout.close();

    ax_releaseParser(&context);
    ax_releaseClassParser(&classContext);
	return 0;
}