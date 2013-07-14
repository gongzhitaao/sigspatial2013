#include <cstring>
#include <cstdlib>
#include <iostream>

#include "core.h"

int main(int argc, char *argv[])
{
    int i = 1;
    double n = 0;
    bool b = true;

    if (0 == _strnicmp(argv[i], "within", 6)) {
        n = atof(argv[++i]);
        b = false;
    }

    std::string point_file = argv[++i];
    std::string polygon_file = argv[++i];
    std::string output_file = argv[++i];

    if (b) inside(point_file, polygon_file, output_file);
    else within(n, point_file, polygon_file, output_file);

    return 0;
}
