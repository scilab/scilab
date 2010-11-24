/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2007-2008 - DIGITEO - Bruno JOFRET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
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
            Location* newloc = const_cast<Location*>(&location_get())->clone();
            return new ColonVar(*newloc);
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

#endif // !AST_COLONVAR_HXX
