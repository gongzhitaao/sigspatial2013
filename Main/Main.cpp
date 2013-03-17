#include <fstream>
#include <iostream>
using namespace std;

#include "Core.h"

int main(int argc, char *argv[])
{
    int i = 1;
    double n;
    if (!strncmp(argv[i], "\"inside\"", 8)) {
    } else if (!strncmp(argv[i], "\"within", 6)) {
        n = atof(argv[++i]);
        ++i; // skip `"'
    }
    string point_file = argv[++i];
    string polygon_file = argv[++i];
    string output_file = argv[++i];

    return 0;
}