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
** \file ast/breakexp.hxx
** Define the Break Expression class.
*/

#ifndef AST_BREAKEXP_HXX
#define AST_BREAKEXP_HXX

#include "controlexp.hxx"

namespace ast
{
/** \brief Abstract an Break Expression node.
**
** \b Example:  break ;*/
class BreakExp : public ControlExp
{
public:
    BreakExp (const Location& location)
        : ControlExp (location)
    {
    }

    /** \brief Destroy an Break Exp node. */
    virtual ~BreakExp ()
    {
    }
    /** \} */

    virtual BreakExp* clone()
    {
        BreakExp* cloned = new BreakExp(getLocation());
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
    virtual ExpType getType() const
    {
        return BREAKEXP;
    }
    inline bool isBreakExp() const
    {
        return true;
    }
};

} // namespace ast


#endif // !AST_BREAKEXP_HXX
