/*!
  \file   utils.h
  \author Zhitao Gong <me@gongzhitaao.org>
  \date   Mon Jul  8 19:59:15 2013

  \brief  Utility package
*/

#ifndef _SIGSPATIAL2013_UTILS_H_
#define _SIGSPATIAL2013_UTILS_H_

#include "defs.h"
#include "polygon.h"

namespace SigSpatial2013 {
    void read_polygon(const std::string &fpoly,
                      std::vector<PolygonSeq> &v,
                      bool within = false);

    const size_t POINT_SIZE = 1e4;

    bool read_point(std::ifstream &f, std::vector<node_t> &v);

    const double PRECISION = 1e-2;

    inline bool fuzzy_eq(const double &a, const double &b)
    { return (-PRECISION < a-b) && (a-b < PRECISION); }
}

#endif /* _SIGSPATIAL2013_UTILS_H_ */
