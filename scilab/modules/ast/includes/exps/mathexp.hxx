/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2007-2008 - DIGITEO - Bruno JOFRET
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

#ifndef __AST_MATHEXP_HXX__
#define __AST_MATHEXP_HXX__

#include "exp.hxx"

namespace ast
{
/** \brief Abstract an Mathematical Expression node.
**
** \b Example:  Operations, Transpose, Matrices... */
class MathExp : public Exp
{
public:
    MathExp (const Location& location)
        : Exp (location)
    {
    }

    virtual ~MathExp ()
    {
    }
};
} // namespace ast
#endif /* !__AST_MATHEXP_HXX__ */
