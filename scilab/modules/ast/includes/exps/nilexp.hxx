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

#ifndef __AST_NILEXP_HXX__
#define __AST_NILEXP_HXX__

#include "constexp.hxx"

namespace ast
{
/** \brief Null expression
**
**/
class NilExp : public ConstExp
{
public:
    NilExp (const Location& location)
        : ConstExp (location)
    {
    }

    virtual ~NilExp ()
    {
    }

    virtual NilExp* clone()
    {
        NilExp* cloned = new NilExp(getLocation());
        cloned->setVerbose(isVerbose());
        return cloned;
    }

    virtual bool equal(const Exp & e) const
    {
        return e.getType() == NILEXP;
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
        return NILEXP;
    }
    inline bool isNilExp() const
    {
        return true;
    }
};
} // namespace ast

#endif /* !__AST_NILEXP_HXX__ */
