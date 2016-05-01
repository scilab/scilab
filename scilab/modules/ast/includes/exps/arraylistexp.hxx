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
** \file arraylistexp.hxx
** Define the Sequence Expression class.
*/

#ifndef __ARRAYLISTEXP_HXX__
#define __ARRAYLISTEXP_HXX__

#include "exp.hxx"

namespace ast
{

/** \brief Abstract an Array List of Expression node.
**
** \b Example: foo(a), foo(b) */
class ArrayListExp : public Exp
{
    /** \name Ctor & dtor.
    ** \{ */
public:
    /** \brief Construct an Array List of Expression node.
    ** \param location scanner position informations
    ** \param body EXP LIST intruction
    */
    ArrayListExp (const Location& location,
                  exps_t& exps)
        : Exp (location)
    {
        for (exps_t::const_iterator it = exps.begin(), itEnd = exps.end(); it != itEnd ; ++it)
        {
            _exps.push_back(*it);
            (*it)->setParent(this);
        }

        delete &exps;
    }

    virtual ArrayListExp* clone()
    {
        exps_t* exps = new exps_t;
        for (exps_t::const_iterator it = _exps.begin(), itEnd = _exps.end(); it != itEnd ; ++it)
        {
            exps->push_back((*it)->clone());
        }

        ArrayListExp* cloned = new ArrayListExp(getLocation(), *exps);
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
    const exps_t& getExps() const
    {
        return _exps;
    }

    exps_t&	getExps()
    {
        return _exps;
    }
    /** \} */


    virtual ExpType getType() const
    {
        return ARRAYLISTEXP;
    }
    inline bool isArrayListExp() const
    {
        return true;
    }
};

} // namespace ast

#endif // __ARRAYLISTEXP_HXX__
