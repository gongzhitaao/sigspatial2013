#pragma once

#include <ppl.h>

#include <string>
#include <fstream>
#include <vector>

bool pip(double x, double y, const std::vector<double> &rx, const std::vector<double> &ry);

//void version1(const std::string &fpt, const std::string &fpoly, const std::string &o);
void version2(const std::string &fpt, const std::string &fpoly, const std::string &o);