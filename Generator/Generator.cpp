// file: Generator.cpp

#include "Generator.h"

#include <iterator>

#include <boost/lexical_cast.hpp>

using namespace std;

Generator::Generator(void)
    : _ptbeg("\
<gml:Point srsName=\"EPSG:54004\" xmlns:gml=\"http://www.opengis.net/gml\">\
  <gml:coordinates decimal=\".\" cs=\",\" ts=\" \">")
    , _ptend("\
  </gml:coordinates>\
</gml:Point>")
    , _obeg("\
<gml:Polygon srsName=\"EPSG:54004\" xmlns:gml=\"http://www.opengis.net/gml\">\
  <gml:outerBoundaryIs>\
    <gml:LinearRing>\
      <gml:coordinates decimal=\".\" cs=\",\" ts=\" \">")
    /* x1,y1 x2,y2 x3,y3 ... */
    , _oend("\
      </gml:coordinates>\
    </gml:LinearRing>\
    </gml:outerBoundaryIs>")
    , _ibeg("\
  <gml:innerBoundaryIs>")
    , _rbeg("\
    <gml:LinearRing>\
      <gml:coordinates decimal=\".\" cs=\",\" ts=\" \">")
    /* x1,y1 x2,y2 x3,y3 ... */
    , _rend("\
      </gml:coordinates>\
    </gml:LinearRing>")
    , _iend("\
  </gml:innerBoundaryIs>\
</gml:Polygon>")
{
}


Generator::~Generator(void)
{
}

string Generator::make_gml_point(string &s, double x, double y)
{
    return s = _ptbeg + boost::lexical_cast<string>(x) + ',' + boost::lexical_cast<string>(y) + _ptend;
}

string Generator::make_gml_poly(string &s,
                                vector<double> &ox, vector<double> &oy,
                                vector<vector<double> > &ix, vector<vector<double> > &iy)
{
    string s1 = _obeg;
    for (int i = 0; i < ox.size(); ++i) {
        s1 += boost::lexical_cast<string>(ox[i]) + ',' +
            boost::lexical_cast<string>(oy[i]) + ' ';
    }
    s1 += _oend;
    string s2 = _ibeg;
    for (int i = 0; i < ix.size(); ++i) {
        string s3 = _rbeg;
        for (int j = 0; j < ix[i].size(); ++j) {
            s3 += boost::lexical_cast<string>(ix[i][j]) + ',' +
                boost::lexical_cast<string>(iy[i][j]) + ' ';
        }
        s3 += _rend;
        s2 += s3;
    }
    s2 += _iend;
    return s = s1 + s2;
}

void Generator::fill_vec_real_n(vector<double> &v, int n)
{
    _eng.seed(random_device()());
    generate_n(back_inserter(v), n, [&]{
        return uniform_real_distribution<>()(_eng); });
}