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

#ifndef __AST_NILEXP_HXX__
#define __AST_NILEXP_HXX__

#include "constexp.hxx"

namespace ast
{
    /** \brief Null expression
    **
    **/
    class NilExp : public ConstExp
    {
    public:
        NilExp (const Location& location) 
            : ConstExp (location)
        {
        }

        virtual ~NilExp ()
        {
        }

        virtual NilExp* clone()
        {
            Location* newloc = const_cast<Location*>(&location_get())->clone();
            NilExp* cloned = new NilExp(*newloc);
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

#endif /* !__AST_NILEXP_HXX__ */
