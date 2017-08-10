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


#ifndef AST_COLONVAR_HXX
#define AST_COLONVAR_HXX

#include "var.hxx"

namespace ast
{
/** \brief Abstract a Colon Variable node.
**
** \b Example: : */
class ColonVar : public Var
{
    /** \name Ctor & dtor.
    ** \{ */
public:
    /** \brief Construct a Colon Variable node.
    ** \param location scanner position informations
    ** \param name the name of the variable
    */
    ColonVar (const Location& location)
        : Var (location)
    {
    }
    /** \brief Destroy a Field Variable node.
    **
    ** Delete name, see constructor. */
    virtual ~ColonVar ()
    {
    }

    virtual ColonVar* clone()
    {
        ColonVar* cloned = new ColonVar(getLocation());
        cloned->setVerbose(isVerbose());
        return cloned;
    }

    virtual bool equal(const Exp & e) const
    {
        return e.getType() == COLONVAR;
    }

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

    virtual ExpType getType() const
    {
        return COLONVAR;
    }
    inline bool isColonVar() const
    {
        return true;
    }

};

} // namespace ast

#endif // !AST_COLONVAR_HXX
