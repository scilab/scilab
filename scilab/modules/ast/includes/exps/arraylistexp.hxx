/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2008-2008 - DIGITEO - Bruno JOFRET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
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
    }

    virtual ArrayListExp* clone()
    {
        exps_t exps;
        for (exps_t::const_iterator it = _exps.begin(), itEnd = _exps.end(); it != itEnd ; ++it)
        {
            exps.push_back((*it)->clone());
        }

        ArrayListExp* cloned = new ArrayListExp(getLocation(), exps);
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


    virtual ExpType getType()
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
