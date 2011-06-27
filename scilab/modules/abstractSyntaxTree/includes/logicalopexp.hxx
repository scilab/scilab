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

#ifndef AST_LOGICALOPEXP_HXX
#define AST_LOGICALOPEXP_HXX

#include <assert.h>
#include "mathexp.hxx"

namespace ast
{

    /** \brief Abstract an Operation Expression node.
    **
    ** \b Example: 77 * 27 */
    class LogicalOpExp : public OpExp
    {

        /** \name Ctor & dtor.
        ** \{ */
    public:
        /** \brief Construct an Operation Expression node.
        ** \param location scanner position informations
        ** \param left left expression of the operator
        ** \param oper operator description
        ** \param right right expression of the operator
        **
        ** \b Example: 77 * 27
        ** \li "77" is the left expression
        ** \li "*" is the operator
        ** \li "27" is the right expression
        */
        LogicalOpExp (const Location& location,
            Exp& left, Oper oper, Exp& right):
        OpExp (location, left, oper, right)
        {
        }

        /** \brief Destroy a Operation Expression node.
        **
        ** Delete left and right, see constructor. */
        virtual ~LogicalOpExp ()
        {
        }
        /** \} */

        virtual LogicalOpExp* clone()
        {
            Location* newloc = const_cast<Location*>(&location_get())->clone();
            LogicalOpExp* cloned = new LogicalOpExp(*newloc, *left_get().clone(), oper_get(), *right_get().clone());
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

        /** \brief set the operator description (read only) */
        void oper_set(Oper oper)
        {
            _oper = oper;
        }

    };

} // namespace ast

#endif // !AST_LOGICALOPEXP_HXX
