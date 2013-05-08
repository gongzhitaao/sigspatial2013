// file: Generator.h

#pragma once

#include <random>
#include <string>
#include <vector>

class Generator
{
public:
    Generator(void);
    ~Generator(void);

    std::string make_gml_point(std::string &s, double x, double y);
    std::string make_gml_poly(std::string &s,
        std::vector<double> &ox, std::vector<double> &oy,
        std::vector<std::vector<double> > &ix, std::vector<std::vector<double> > &iy);

    void fill_vec_real_n(std::vector<double> &v, int n);

private:
    const std::string _ptbeg, _ptend;
    const std::string _obeg, _oend, _ibeg, _rbeg, _rend, _iend;

    std::mt19937 _eng;
};

