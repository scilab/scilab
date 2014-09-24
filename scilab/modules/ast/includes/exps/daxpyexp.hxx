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

};
} // namespace ast
#endif /* !AST_DAXPYEXP_HXX */
