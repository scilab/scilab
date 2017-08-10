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

#ifndef AST_NOTEXP_HXX
#define AST_NOTEXP_HXX

#include "mathexp.hxx"

namespace ast
{
/** \brief Abstract an Operation Expression node.
**
** \b Example: ~a */
class NotExp : public MathExp
{
public:
    /** \name Ctor & dtor.
    ** \{ */
public:
    /** \brief Construct an Operation Expression node.
    ** \param location scanner position informations
    ** \param exp expression of the NOT operator
    **
    ** \b Example: ~a
    */
    NotExp (const Location& location,
            Exp& exp)
        : MathExp (location)
    {
        exp.setParent(this);
        _exps.push_back(&exp);
    }

    /** \brief Destroy a Not Operation Expression node.
    **
    ** Delete expression, see constructor. */
    virtual ~NotExp ()
    {
    }
    /** \} */

    virtual NotExp* clone()
    {
        NotExp* cloned = new NotExp(getLocation(), *getExp().clone());
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
    /** \brief Return the expression of the operation (read only) */
    const Exp& getExp() const
    {
        return *_exps[0];
    }
    /** \brief Return the expression of the operation (read and write) */
    Exp& getExp()
    {
        return *_exps[0];
    }

    virtual ExpType getType() const
    {
        return NOTEXP;
    }
    inline bool isNotExp() const
    {
        return true;
    }
};

} // namespace ast

#endif // !AST_NOTEXP_HXX
