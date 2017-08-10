/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2014 - Scilab Enterprises - Antoine ELIAS
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

    virtual ExpType getType() const
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
