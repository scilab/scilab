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

#ifndef AST_LOGICALOPEXP_HXX
#define AST_LOGICALOPEXP_HXX

#include <assert.h>
#include "mathexp.hxx"

namespace ast
{

/** \brief Abstract an Operation Expression node.
**
** \b Example: 77 * 27 */
class LogicalOpExp : public OpExp
{

    /** \name Ctor & dtor.
    ** \{ */
public:
    /** \brief Construct an Operation Expression node.
    ** \param location scanner position informations
    ** \param left left expression of the operator
    ** \param oper operator description
    ** \param right right expression of the operator
    **
    ** \b Example: 77 * 27
    ** \li "77" is the left expression
    ** \li "*" is the operator
    ** \li "27" is the right expression
    */
    LogicalOpExp (const Location& location,
                  Exp& left, Oper oper, Exp& right):
        OpExp (location, left, oper, right)
    {
    }

    /** \brief Destroy a Operation Expression node.
    **
    ** Delete left and right, see constructor. */
    virtual ~LogicalOpExp ()
    {
    }
    /** \} */

    virtual LogicalOpExp* clone()
    {
        LogicalOpExp* cloned = new LogicalOpExp(getLocation(), *getLeft().clone(), getOper(), *getRight().clone());
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

    /** \brief set the operator description (read only) */
    void setOper(Oper oper)
    {
        _oper = oper;
    }

    virtual ExpType getType() const
    {
        return LOGICALOPEXP;
    }
    inline bool isLogicalOpExp() const
    {
        return true;
    }
};

} // namespace ast

#endif // !AST_LOGICALOPEXP_HXX
