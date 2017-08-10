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

#ifndef AST_DAXPYEXP_HXX
#define AST_DAXPYEXP_HXX

#include "optimizedexp.hxx"
#include "internal.hxx"

namespace ast
{
class DAXPYExp : public OptimizedExp
{
public:
    DAXPYExp (const Location& location, Exp& a, Exp& x, Exp& y)
        : OptimizedExp (location)
    {
        _exps.push_back(&a);
        _exps.push_back(&x);
        _exps.push_back(&y);

        a.setParent(this);
        x.setParent(this);
        y.setParent(this);
    }

    virtual ~DAXPYExp ()
    {
        //all children are ref so let original parent to destroy then
        _exps.clear();
    }

    virtual DAXPYExp* clone()
    {
        DAXPYExp* cloned = new DAXPYExp(getLocation(), *getA().clone(), *getX().clone(), *getY().clone());
        return cloned;
    }

    virtual void accept (Visitor& v)
    {
        v.visit (*this);
    }

    virtual void accept (ConstVisitor& v) const
    {
        v.visit (*this);
    }

    const Exp& getA() const
    {
        return *_exps[0];
    }

    Exp& getA()
    {
        return *_exps[0];
    }

    const Exp& getX() const
    {
        return *_exps[1];
    }

    Exp& getX()
    {
        return *_exps[1];
    }

    const Exp& getY() const
    {
        return *_exps[2];
    }

    Exp& getY()
    {
        return *_exps[2];
    }

    virtual ExpType getType() const
    {
        return DAXPYEXP;
    }

};
} // namespace ast
#endif /* !AST_DAXPYEXP_HXX */
