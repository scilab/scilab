/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2006-2008 - DIGITEO - Bruno JOFRET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef AST_IFEXP_HXX
# define AST_IFEXP_HXX

# include <assert.h>
# include "controlexp.hxx"

namespace ast
{

    /*
    ** \brief Abstract an If Expression node.
    **
    ** \b Example: if (2097 = 0) then disp("fail") else disp("OK")
    */
    class IfExp : public ControlExp
    {
    public:
        /*
        ** \brief If Expression kind.
        **
        ** \brief Specify if the If Expression has a value (set at type checking).
        */
        enum Kind
        {
            // \brief Invalid kind
            invalid_kind,
            // \brief "Is an instruction" (no value) kind
            instruction_kind,
            // \brief "Is an expression" (value) kind
            expression_kind
        };

        // \brief Ctor & dtor.
    public:
        /*
        ** \brief Construct an If Expression node.
        ** \param location scanner position informations
        ** \param test chose the instruction to execute
        ** \param then instruction if test is true
        ** \param else instruction if test is false
        */
        IfExp(const Location& location,
              Exp& test, Exp& t, Exp& e) :
            ControlExp (location),
            _test (&test),
            _then (&t),
            _else (&e),
            _has_else (true)
            {
            }

        IfExp(const Location& location,
              Exp& test, Exp& t) :
            ControlExp (location),
            _test (&test),
            _then (&t),
            _else (new ast::CommentExp(location, new std::wstring(L"No else !!"))),
            // For first display in order to see what appends
            _has_else (false)
            {
            }

        virtual ~IfExp()
            {
                delete _test;
                delete _then;
                delete _else;
            }

        // \brief Visitors entry point.
    public:
        virtual void	accept(Visitor& v)
            {
                v.visit (*this);
            }
        virtual void	accept(ConstVisitor& v) const
            {
                v.visit (*this);
            }

        // \brief Accessors.
    public:
        // \brief Return the select condition of the loop (read only).
        const Exp&	test_get() const
            {
                return *_test;
            }
        // \brief Return the select condition of the loop (read and write).
        Exp&	test_get()
            {
                return *_test;
            }

        // \brief Return the intructions if test is true (read only).
        const Exp&	then_get() const
            {
                return *_then;
            }
        // \brief Return the instructions if test is true (read and write).
        Exp& then_get()
            {
                return *_then;
            }

        // \brief Return the instruction if test is false (read only).
        const Exp&	else_get() const
            {
                return *_else;
            }
        // \brief Return the instruction if test is false (read and write).
        Exp&	else_get()
            {
                return *_else;
            }

        // \brief Return if there is an else body
        bool	has_else()
            {
                return _has_else;
            }
        // \brief Return if there is an else body
        bool	has_else() const
            {
                return _has_else;
            }

        // \brief Return the kind of the If Expression (read only)
        Kind kind_get (void) const
            {
                assert (_kind != invalid_kind);
                return _kind;
            }
        // \brief Set the kind of the If Expression
        void kind_set (Kind kind)
            {
                this->_kind = kind;
            }

    protected:
        // \brief "has a value" qualifier.
        Exp*	_test;
        Exp*	_then;
        Exp*	_else;
        Kind	_kind;
        bool	_has_else;
    };

} // namespace ast

#endif // !AST_IFEXP_HXX
