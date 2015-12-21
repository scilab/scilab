/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2007-2008 - DIGITEO - Bruno JOFRET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
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
