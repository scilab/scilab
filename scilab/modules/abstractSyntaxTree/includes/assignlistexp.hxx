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
            std::list<Exp *>& exps) :
        ArrayListExp (location, exps)
        {
        }

        virtual AssignListExp* clone()
        {
            std::list<Exp *>* exps = new std::list<Exp *>;
            std::list<Exp *>::const_iterator it;
            for(it = _exps->begin() ; it != _exps->end() ; it++)
            {
                exps->push_back((*it)->clone());
            }

            Location* newloc = const_cast<Location*>(&location_get())->clone();
            AssignListExp* cloned = new AssignListExp(*newloc, *exps);
            cloned->set_verbose(is_verbose());
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

    };

} // namespace ast

#endif // __ASSIGNLISTEXP_HXX__
