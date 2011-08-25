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
** \file ast/breakexp.hxx
** Define the Break Expression class.
*/

#ifndef AST_BREAKEXP_HXX
#define AST_BREAKEXP_HXX

#include "controlexp.hxx"

namespace ast
{
    /** \brief Abstract an Break Expression node.
    **
    ** \b Example:  break ;*/
    class BreakExp : public ControlExp
    {
    public:
        BreakExp (const Location& location) 
            : ControlExp (location)
        {
        }

        /** \brief Destroy an Break Exp node. */
        virtual ~BreakExp ()
        {
        }
        /** \} */

        virtual BreakExp* clone()
        {
            Location* newloc = const_cast<Location*>(&location_get())->clone();
            BreakExp* cloned = new BreakExp(*newloc);
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


#endif // !AST_BREAKEXP_HXX
