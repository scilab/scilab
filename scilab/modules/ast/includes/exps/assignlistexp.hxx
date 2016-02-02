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
 ** \file assignlistexp.hxx
 ** Define the Sequence Expression class.
 */

#ifndef __ASSIGNLISTEXP_HXX__
#define __ASSIGNLISTEXP_HXX__

#include "assignlistexp.hxx"

namespace ast
{
/** \brief Abstract an Assign List of Expression node.
**
** \b Example: [a, b, c.d, e(2)] = ... */
class AssignListExp : public ArrayListExp
{
    /** \name Ctor & dtor.
    ** \{ */
public:
    /** \brief Construct an Assign List of Expression node.
    ** \param location scanner position informations
    ** \param body EXP LIST intruction
    */
    AssignListExp (const Location& location,
                   exps_t& exps) :
        ArrayListExp (location, exps)
    {
    }

    virtual AssignListExp* clone()
    {
        exps_t* exps = new exps_t;
        for (exps_t::const_iterator it = _exps.begin() ; it != _exps.end() ; ++it)
        {
            exps->push_back((*it)->clone());
        }

        AssignListExp* cloned = new AssignListExp(getLocation(), *exps);
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
        return ASSIGNLISTEXP;
    }
    inline bool isAssignListExp() const
    {
        return true;
    }
};

} // namespace ast

#endif // __ASSIGNLISTEXP_HXX__
