#ifndef _SIG_CORE_H_
#define _SIG_CORE_H_

#pragma once

#include <string>
#include <vector>

bool pip(double x, double y,
         const std::vector<double> &rx,
         const std::vector<double> &ry);

void ver2(const std::string &fpt,
          const std::string &fpoly,
          const std::string &fo);

#endif /* _SIG_CORE_H_ */
