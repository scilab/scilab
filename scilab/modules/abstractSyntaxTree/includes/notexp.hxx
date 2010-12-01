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

#ifndef AST_NOTEXP_HXX
#define AST_NOTEXP_HXX

#include "mathexp.hxx"

namespace ast
{
    /** \brief Abstract an Operation Expression node.
    **
    ** \b Example: ~a */
    class NotExp : public MathExp
    {
    public:
        /** \name Ctor & dtor.
        ** \{ */
    public:
        /** \brief Construct an Operation Expression node.
        ** \param location scanner position informations
        ** \param exp expression of the NOT operator
        **
        ** \b Example: ~a
        */
        NotExp (const Location& location,
            Exp& exp)
            : MathExp (location),
            _exp (&exp)
        {
        }

        /** \brief Destroy a Not Operation Expression node.
        **
        ** Delete expression, see constructor. */
        virtual ~NotExp ()
        {
            delete _exp;
        }
        /** \} */

        virtual NotExp* clone()
        {
            Location* newloc = const_cast<Location*>(&location_get())->clone();
            return new NotExp(*newloc, *exp_get().clone());
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
        /** \brief Return the expression of the operation (read only) */
        const Exp& exp_get () const
        {
            return *_exp;
        }
        /** \brief Return the expression of the operation (read and write) */
        Exp& exp_get ()
        {
            return *_exp;
        }

    protected:
        /** \brief Left expression of the operation. */
        Exp* _exp;
    };

} // namespace ast

#endif // !AST_NOTEXP_HXX
