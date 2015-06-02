/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2015 - Scilab Enterprises - Calixte DENIZET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __FAST_SELECT_EXP_HXX__
#define __FAST_SELECT_EXP_HXX__

#include "internal.hxx"

namespace ast
{
class FastSelectExp : public SelectExp
{

public:

    FastSelectExp(const Location& location,
                  Exp& select,
                  exps_t& cases,
                  Exp& defaultCase) : SelectExp(location, select, cases, defaultCase) { }

    FastSelectExp(const Location& location,
                  Exp& select,
                  exps_t& cases) : SelectExp(location, select, cases) { }

    virtual ~FastSelectExp()
    {
        _exps.clear();
    }

    inline bool isFastSelectExp() const
    {
        return true;
    }
};

} // namespace ast

#endif // __FAST_SELECT_EXP_HXX__
