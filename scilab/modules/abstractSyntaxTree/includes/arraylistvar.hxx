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
            std::list<Var *>& vars) 
            : Var (location),
            _vars (&vars)
        {
        }

        virtual ~ArrayListVar ()
        {
            delete _vars;
        }

        virtual ArrayListVar* clone()
        {
            std::list<Var *>* vars = new std::list<Var *>;
            std::list<Var *>::const_iterator it;
            for(it = vars_get().begin() ; it != vars_get().end() ; it++)
            {
                Var* var = dynamic_cast<Var*>((*it)->clone());
                vars->push_back(var);
            }

            Location* newloc = const_cast<Location*>(&location_get())->clone();
            ArrayListVar* cloned = new ArrayListVar(*newloc, *vars);
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
        const std::list<Var *>&	vars_get() const
        {
            return *_vars;
        }

        std::list<Var *>&	vars_get()
        {
            return *_vars;
        }
        /** \} */


    protected:
        std::list<Var *>* _vars;
    };

} // namespace ast

#endif // __ARRAYLISTVAR_HXX__
