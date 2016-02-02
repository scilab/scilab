/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2010-2010 - DIGITEO - Antoine ELIAS
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
 ** \file cellcallexp.hxx
 ** Define the Sequence Expression class.
 */

#ifndef AST_CELLCALLEXP_HXX
#define AST_CELLCALLEXP_HXX

#include "callexp.hxx"

namespace ast
{

/** \brief Abstract an Array List of Expression node.
**
** \b Example: foo(a), foo(b) */
class CellCallExp : public CallExp
{
    /** \name Ctor & dtor.
    ** \{ */
public:
    /*
    ** \brief Construct a CellCall Expression node.
    ** \param location scanner position informations
    ** \param name of the cell
    ** \param list of the arguments
    */
    CellCallExp (const Location& location,
                 Exp& name,
                 exps_t& args)
        : CallExp(location, name, args)
    {
    }

    virtual CellCallExp* clone()
    {
        exps_t* args = new ast::exps_t;
        for (exps_t::const_iterator it = ++(_exps.begin()); it != _exps.end() ; ++it)
        {
            args->push_back((*it)->clone());
        }

        CellCallExp* cloned = new CellCallExp(getLocation(), *getName().clone(), *args);
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
        return CELLCALLEXP;
    }
    inline bool isCellCallExp() const
    {
        return true;
    }

};

} // namespace ast

#endif // !AST_CELLCALLEXP_HXX
