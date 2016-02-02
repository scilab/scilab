/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2008-2008 - DIGITEO - Bruno JOFRET
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

/**
 ** \file returnexp.hxx
 ** Define the Return Expression class.
 */

#ifndef AST_RETURNEXP_HXX
#define AST_RETURNEXP_HXX

#include "controlexp.hxx"
#include "commentexp.hxx"

namespace ast
{
/** \brief Abstract a Return Expression node.
**
** \b Example: return or return plop */
class ReturnExp : public ControlExp
{
    /** \name Ctor & dtor.
    ** \{ */
public:
    /** \brief Construct a Return Expression node.
    ** \param location scanner position informations
    ** \param exp the returned exp
    */
    ReturnExp (const Location& location, Exp* exp = NULL)
        : ControlExp (location),
          _is_global(true)
    {
        if (exp)
        {
            _is_global = false;
            exp->setParent(this);
            _exps.push_back(exp);
        }
        else
        {
            _exps.push_back(new ast::CommentExp(location, new std::wstring(L"No return !!")));
            _exps[0]->setParent(this);
        }

    }

    virtual ~ReturnExp ()
    {
    }

    virtual ReturnExp* clone()
    {
        ReturnExp* cloned = NULL;
        if (isGlobal())
        {
            cloned = new ReturnExp(getLocation());
        }
        else
        {
            cloned = new ReturnExp(getLocation(), getExp().clone());
        }

        cloned->setVerbose(isVerbose());
        return cloned;
    }

    /** \name Visitors entry point.
    ** \{ */
public:
    /** \brief Accept a const visitor \a v. */
    virtual void accept (Visitor& v)
    {
        v.visit (*this);
    }
    /** \brief Accept a non-const visitor \a v. */
    virtual void accept (ConstVisitor& v) const
    {
        v.visit (*this);
    }
    /** \} */

    /** \name Accessors.
    ** \{ */
public:
    const Exp &	getExp() const
    {
        return *_exps[0];
    }

    Exp &	getExp()
    {
        return *_exps[0];
    }

    bool isGlobal() const
    {
        return _is_global;
    }

    virtual ExpType getType() const
    {
        return RETURNEXP;
    }
    inline virtual bool isReturnExp() const
    {
        return true;
    }
protected:
    bool	_is_global;
};

} // namespace ast

#endif // AST_RETURNEXP_HXX
