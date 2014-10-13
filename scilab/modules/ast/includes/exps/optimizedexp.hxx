/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2014 - Scilab Enterprises - Antoine ELIAS
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef AST_OPTIMIZEDEXP_HXX
#define AST_OPTIMIZEDEXP_HXX

#include "exp.hxx"

namespace ast
{
class OptimizedExp : public Exp
{
public:
    OptimizedExp (const Location& location)
        : Exp (location)
    {
    }

    virtual ~OptimizedExp ()
    {
    }

    virtual ExpType getType()
    {
        return OPTIMIZEDEXP;
    }

    inline bool isOptimizedExp() const
    {
        return true;
    }
};
} // namespace ast
#endif /* !AST_OPTIMIZEDEXP_HXX */
