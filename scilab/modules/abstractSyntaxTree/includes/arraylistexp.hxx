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
            std::list<Exp *>& exps) 
            : Exp (location),
            _exps (&exps)
        {
        }

        virtual ~ArrayListExp ()
        {
            delete _exps;
        }

        virtual ArrayListExp* clone()
        {
            std::list<Exp *>* exps = new std::list<Exp *>;
            std::list<Exp *>::const_iterator it;
            for(it = _exps->begin() ; it != _exps->end() ; it++)
            {
                exps->push_back((*it)->clone());
            }

            Location* newloc = const_cast<Location*>(&location_get())->clone();
            ArrayListExp* cloned = new ArrayListExp(*newloc, *exps);
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


        /** \name Accessors.
        ** \{ */
    public:
        const std::list<Exp *>&	exps_get() const
        {
            return *_exps;
        }

        std::list<Exp *>&	exps_get()
        {
            return *_exps;
        }
        /** \} */


    protected:
        std::list<Exp *>* _exps;
    };

} // namespace ast

#endif // __ARRAYLISTEXP_HXX__
