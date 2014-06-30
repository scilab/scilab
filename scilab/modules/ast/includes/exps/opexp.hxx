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

#ifndef AST_OPEXP_HXX
#define AST_OPEXP_HXX

#include <assert.h>
#include "mathexp.hxx"

namespace ast
{

/** \brief Abstract an Operation Expression node.
 **
 ** \b Example: 77 * 27 */
class OpExp : public MathExp
{
public:
    /** \brief Operator qualifier */
    enum Oper
    {
        // Arithmetics.
        /** \brief "+" */			plus,
        /** \brief "-" */			minus,
        /** \brief "*" */			times,
        /** \brief "/" */			rdivide,
        /** \brief "\" */			ldivide,
        /** \brief "**" or "^" */		power,

        // Element Ways.
        /** \brief ".*" */		dottimes,
        /** \brief "./" */		dotrdivide,
        /** \brief ".\" */		dotldivide,
        /** \brief ".^" */		dotpower,

        // Kroneckers
        /** \brief ".*." */		krontimes,
        /** \brief "./." */		kronrdivide,
        /** \brief ".\." */		kronldivide,

        // Control
        // FIXME : What the hell is this ???
        /** \brief "*." */		controltimes,
        /** \brief "/." */		controlrdivide,
        /** \brief "\." */		controlldivide,

        // Comparison.
        /** \brief "==" */		eq,
        /** \brief "<>" or "~=" */	ne,
        /** \brief "<" */			lt,
        /** \brief "<=" */		le,
        /** \brief "<" */			gt,
        /** \brief ">=" */		ge,

        // Logical operators
        /** \brief "&" */		logicalAnd,
        /** \brief "|" */		logicalOr,
        /** \brief "&&" */	logicalShortCutAnd,
        /** \brief "||" */	logicalShortCutOr,

        // Unary minus
        /** \brief "-" */ unaryMinus
    };

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
    OpExp (const Location& location,
           Exp& left, Oper oper, Exp& right)
        : MathExp (location),
          _left (&left),
          _oper (oper),
          _right (&right)
    {
    }

    /** \brief Destroy a Operation Expression node.
    **
    ** Delete left and right, see constructor. */
    virtual ~OpExp ()
    {
        delete _left;
        delete _right;
    }
    /** \} */

    virtual OpExp* clone()
    {
        OpExp* cloned = new OpExp(location_get(), *left_get().clone(), oper_get(), *right_get().clone());
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


    /** \name Setters.
    ** \{ */
public :
    virtual void left_set(Exp& left)
    {
        _left = &left;
    }
    /** \} */


    /** \name Accessors.
    ** \{ */
public:
    /** \brief Return the left expression of the operation (read only) */
    const Exp& left_get () const
    {
        return *_left;
    }
    /** \brief Return the left expression of the operation (read and write) */
    Exp& left_get ()
    {
        return *_left;
    }

    /** \brief Return the operator description (read only) */
    Oper oper_get () const
    {
        return _oper;
    }

    /** \brief Return the right expression of the operation (read only) */
    const Exp& right_get () const
    {
        return *_right;
    }
    /** \brief Return the right expression of the operation (read and write) */
    Exp& right_get ()
    {
        return *_right;
    }


protected:
    /** \brief Left expression of the operation. */
    Exp* _left;
    /** \brief Operator. */
    Oper _oper;
    /** \brief Right expression of the operation. */
    Exp* _right;
};

} // namespace ast

#endif // !AST_OPEXP_HXX
