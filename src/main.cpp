#include <cstring>
#include <fstream>
#include <iostream>

#include "core.h"

int main(int argc, char *argv[])
{
    int i = 1;
    double n;
    bool b = true;
    if (!strncmp(argv[i], "\"INSIDE\"", 8)) {
    } else if (!strncmp(argv[i], "\"WITHIN", 6)) {
        n = atof(argv[++i]);
        ++i; // skip `"'
        b = false;
    }
    std::string point_file = argv[++i];
    std::string polygon_file = argv[++i];
    std::string output_file = argv[++i];

    if (b) inside(point_file, polygon_file, output_file);
    else within(n, point_file, polygon_file, output_file);

    return 0;
}
