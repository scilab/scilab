/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2010-2010 - DIGITEO - Bruno JOFRET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/**
 ** \file ast/continueexp.hxx
 ** Define the Continue Expression class.
 */

#ifndef AST_CONTINUEEXP_HXX
#define AST_CONTINUEEXP_HXX

#include "controlexp.hxx"

namespace ast
{
    /** \brief Abstract an Continue Expression node.
    **
    ** \b Example:  continue ;*/
    class ContinueExp : public ControlExp
    {
    public:
        ContinueExp (const Location& location) 
            : ControlExp (location)
        {
        }

        /** \brief Destroy an Continue Exp node. */
        virtual ~ContinueExp ()
        {
        }
        /** \} */

        virtual ContinueExp* clone()
        {
            Location* newloc = const_cast<Location*>(&location_get())->clone();
            ContinueExp* cloned = new ContinueExp(*newloc);
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


#endif // !AST_CONTINUEEXP_HXX
