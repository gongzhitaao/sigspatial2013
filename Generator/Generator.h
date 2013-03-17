// file: Generator.h

#pragma once

#include <random>
#include <string>

class Generator
{
public:
    Generator(void);
    ~Generator(void);

    std::string make_gml_point(std::string &s, double x, double y);
    std::string make_gml_poly(std::string &s,
        vector<double> &ox, vector<double> &oy,
        vector<vector<double> > &ix, vector<vector<double> > &iy);

private:
    const std::string _ptbeg, _ptend;
    const std::string _polybeg, _obeg, _oend, _ibeg, _iend;
};

