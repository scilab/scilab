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

/**
 ** \file ast/whileexp.hxx
 ** Define the While Expression class.
 */

#ifndef AST_WHILEEXP_HXX
#define AST_WHILEEXP_HXX

#include "controlexp.hxx"

namespace ast
{
    /** \brief Abstract a For Expression node.
    **
    ** \b Example: while (2097 <> 1) do print("WipeOut") */
    class WhileExp : public ControlExp
    {
        /** \name Ctor & dtor.
        ** \{ */
    public:
        /** \brief Construct a While Expression node.
        ** \param location scanner position informations
        ** \param test exit condition of the loop
        ** \param body instructions executed in the loop
        **
        ** \b Example: while (2097 <> 1) do print("WipeOut")
        ** \li "(2097 <> 1)" is the exit condition (allways true here)
        ** \li "print("WipeOut")" body of the loop
        */
        WhileExp (const Location& location,
            Exp& test, Exp& body)
            : ControlExp (location),
            _test (&test),
            _body (&body)
        {
        }

        /** \brief Destroy a While Expression node.
        **
        ** Delete test and body (see constructor). */
        virtual ~WhileExp ()
        {
            delete _test;
            delete _body;
        }
        /** \}*/

        virtual WhileExp* clone()
        {
            Location* newloc = const_cast<Location*>(&location_get())->clone();
            return new WhileExp(*newloc, *test_get().clone(), *body_get().clone());
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
        /** \brief Return the exit condition of the loop (read only). */
        const Exp& test_get() const
        {
            return *_test;
        }
        /** \brief Return the exit condition of the loop (read and write). */
        Exp& test_get()
        {
            return *_test;
        }

        /** \brief Return the body of the loop (read only). */
        const Exp& body_get() const
        {
            return *_body;
        }
        /** \brief Return the body of the loop (read and write). */
        Exp& body_get()
        {
            return *_body;
        }
        /** \} */

    protected:
        /** \brief Exit condition of the loop. */
        Exp* _test;
        /** \brief Instructions executed in the loop. */
        Exp* _body;
    };

} // namespace ast

#endif // !AST_WHILEEXP_HXX
