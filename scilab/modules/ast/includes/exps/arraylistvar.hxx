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
** \file arraylistvar.hxx
** Define the Arraylistuence Varression class.
*/

#ifndef __ARRAYLISTVAR_HXX__
#define __ARRAYLISTVAR_HXX__

#include "var.hxx"

namespace ast
{

/** \brief Abstract an Array List of Variable node.
**
** \b Example: a, b, $, : */
class ArrayListVar : public Var
{
    /** \name Ctor & dtor.
    ** \{ */
public:
    /** \brief Construct an Array List of Variable node.
    ** \param location scanner position informations
    ** \param body VAR LIST intruction
    */
    ArrayListVar (const Location& location,
                  exps_t& vars)
        : Var (location)
    {
        for (auto var : vars)
        {
            var->setParent(this);
            _exps.push_back(var);
        }

        delete &vars;
    }

    virtual ArrayListVar* clone()
    {
        exps_t* exps = new exps_t;
        for (exps_t::const_iterator it = _exps.begin(), itEnd = _exps.end(); it != itEnd ; ++it)
        {
            exps->push_back((*it)->clone());
        }

        ArrayListVar* cloned = new ArrayListVar(getLocation(), *exps);
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

    inline const exps_t& getVars() const
    {
        return _exps;
    }

    inline exps_t& getVars()
    {
        return _exps;
    }
    /** \} */

    virtual ExpType getType() const
    {
        return ARRAYLISTVAR;
    }
    inline bool isArrayListVar() const
    {
        return true;
    }

protected:
};

} // namespace ast

#endif // __ARRAYLISTVAR_HXX__
