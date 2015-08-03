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

#ifndef __INT_SELECT_EXP_HXX__
#define __INT_SELECT_EXP_HXX__

#include "internal.hxx"
#include <stdint.h>

namespace ast
{
class IntSelectExp : public FastSelectExp
{

public:

    IntSelectExp(const Location& location,
                 Exp& select,
                 exps_t& cases,
                 Exp& defaultCase) : FastSelectExp(location, select, cases, defaultCase) { }

    IntSelectExp(const Location& location,
                 Exp& select,
                 exps_t& cases) : FastSelectExp(location, select, cases) { }

    virtual ~IntSelectExp()
    {
    }

    virtual void accept(Visitor& v)
    {
        v.visit(*this);
    }

    virtual void accept(ConstVisitor& v) const
    {
        v.visit(*this);
    }

    virtual Exp * getExp(const int64_t key) const = 0;

    inline bool isIntSelectExp() const
    {
        return true;
    }
};

} // namespace ast

#endif // __INT_SELECT_EXP_HXX__
