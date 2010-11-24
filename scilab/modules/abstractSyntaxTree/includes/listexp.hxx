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

#ifndef AST_LISTEXP_HXX
#define AST_LISTEXP_HXX

#include "exp.hxx"

namespace ast
{

    /** \brief Abstract an Operation Expression node.
    **
    ** \b Example: 1:0.5:10 */
    class ListExp : public Exp
    {
    public:
        /** \name Ctor & dtor.
        ** \{ */
    public:
        /** \brief Construct an Implicit list Expression node.
        ** \param location scanner position informations
        ** \param start the start point of the list
        ** \param step the step between each element
        ** \param end the end of the list (the max value)
        **
        ** \b Example: start:step:stop
        ** \li "start" is the start point of the list
        ** \li "step" is the step between each element
        ** \li "stop" is the end of the list (the max value)
        */
        ListExp (const Location& location,
            Exp& start, Exp& step, Exp& end)
            : Exp (location),
            _start (&start),
            _step (&step),
            _end (&end)
        {
        }

        /** \brief Destroy a Operation Expression node.
        **
        ** Delete left and right, see constructor. */
        virtual ~ListExp ()
        {
            delete _start;
            delete _step;
            delete _end;
        }
        /** \} */

        virtual ListExp* clone()
        {
            Location* newloc = const_cast<Location*>(&location_get())->clone();
            return new ListExp(*newloc, *start_get().clone(), *step_get().clone(), *end_get().clone());
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
        /** \brief Return the expression (read only) */
        const Exp& start_get () const
        {
            return *_start;
        }
        /** \brief Return the expression (read and write) */
        Exp& start_get ()
        {
            return *_start;
        }

        /** \brief Return the expression (read only) */
        const Exp& step_get () const
        {
            return *_step;
        }
        /** \brief Return the expression (read and write) */
        Exp& step_get ()
        {
            return *_step;
        }

        /** \brief Return the expression (read only) */
        const Exp& end_get () const
        {
            return *_end;
        }
        /** \brief Return the expression (read and write) */
        Exp& end_get ()
        {
            return *_end;
        }

        /** \} */

    protected:
        /** \brief start expression of the list. */
        Exp* _start;
        /** \brief step expression of the list. */
        Exp* _step;
        /** \brief end expression of the list. */
        Exp* _end;
    };

} // namespace ast

#endif // !AST_LISTEXP_HXX
