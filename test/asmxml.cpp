#include <gtest/gtest.h>

#include <fstream>
#include <string>

#include "../asm-xml.h"

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


TEST(asmxml, leak)
{
    ax_initialize((void*)malloc, (void*)free);
    AXClassContext classContext;
    ax_initializeClassParser(&classContext);
    AXElementClass *rootClass =
        ax_classFromString(PointSchema, &classContext);

    AXParseContext context;
    ax_initializeParser(&context, 16384);

    std::ifstream fin("../in/points500.cpp");
    std::string line;

    while (getline(fin, line)) {
        AXElement* root = ax_parse(&context, line.c_str(), rootClass, 1);
    }

    fin.close();

    ax_releaseParser(&context);
    ax_releaseClassParser(&classContext);
}
