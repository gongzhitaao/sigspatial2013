/*!
  \file   core.h
  \author Zhitao Gong <me@gongzhitaao.org>
  \date   Mon Jun  3 14:07:10 2013

  \brief  Core functions declaration included.
*/
#ifndef _SIG_CORE_H_
#define _SIG_CORE_H_

#pragma once

#include <string>

/*! \brief INSIDE predicate checking.

  \param fpt input point file
  \param fpoly input polygon file
  \param fo output file
*/
void inside(const std::string &fpt,
            const std::string &fpoly,
            const std::string &fo);

/*! \brief WITHIN_n predicate checking

  \param n distance
  \param fpt input point file
  \param fpoly input polygon file
  \param fo output file
*/
void within(double n,
            const std::string &fpt,
            const std::string &fpoly,
            const std::string &fo);

#endif /* _SIG_CORE_H_ */
