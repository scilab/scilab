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

#ifndef AST_CONSTEXP_HXX
#define AST_CONSTEXP_HXX

#include "exp.hxx"
#include "internal.hxx"

namespace ast
{
/** \brief Abstract an Constant Expression node.
**
** \b Example:  true or 2 or 1,23*/
class ConstExp : public Exp
{
public:
    ConstExp (const Location& location)
        : Exp (location), constant(NULL)
    {
    }

    virtual ~ConstExp ()
    {
        if (constant)
        {
            constant->DecreaseRef();
            constant->killMe();
        }
    }

    inline bool isConstExp() const
    {
        return true;
    }

    types::InternalType* getConstant() const
    {
        return constant;
    }

    types::InternalType* getConstant()
    {
        return constant;
    }

    void setConstant(types::InternalType* _const)
    {
        if (constant)
        {
            constant->DecreaseRef();
            constant->killMe();
        }

        constant = _const;
        constant->IncreaseRef();
    }

protected :
    types::InternalType* constant;
};
} // namespace ast
#endif
