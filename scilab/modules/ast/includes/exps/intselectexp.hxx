/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2015 - Scilab Enterprises - Calixte DENIZET
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
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
