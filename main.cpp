#include <cstring>
#include <fstream>
#include <iostream>

#include "core.h"

int main(int argc, char *argv[])
{
    int i = 1;
    double n;
    if (!strncmp(argv[i], "\"inside\"", 8)) {
    } else if (!strncmp(argv[i], "\"within", 6)) {
        n = atof(argv[++i]);
        ++i; // skip `"'
    }
    std::string point_file = argv[++i];
    std::string polygon_file = argv[++i];
    std::string output_file = argv[++i];

    inside(point_file, polygon_file, output_file);

    return 0;
}
