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
