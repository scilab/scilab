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

#ifndef AST_CONTROLEXP_HXX
#define AST_CONTROLEXP_HXX

#include "exp.hxx"
#include "seqexp.hxx"

namespace ast
{
/** \brief Abstract an Control Sequence Expression node.
**
** \b Example:  if...then...else...end */
class ControlExp : public Exp
{
public:
    ControlExp (const Location& location)
        : Exp (location)
    {
    }

    virtual ~ControlExp ()
    {
    }

    //forward continuable information to children
    virtual inline void setContinuable(void)
    {
        Exp::setContinuable();
        for (auto exp : _exps)
        {
            exp->setContinuable();
        }
    }

    //forward returnable information to children
    virtual inline void setReturnable(void)
    {
        Exp::setReturnable();
        for (auto exp : _exps)
        {
            exp->setReturnable();
        }
    }

    //forward breakable information to children
    virtual inline void setBreakable(void)
    {
        Exp::setBreakable();
        for (auto exp : _exps)
        {
            exp->setBreakable();
        }
    }

    inline bool isControlExp() const
    {
        return true;
    }

};
} // namespace ast
#endif /* !AST_CONTROLEXP_HXX */
