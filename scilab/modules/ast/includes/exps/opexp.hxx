/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2007-2008 - DIGITEO - Bruno JOFRET
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
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
          _oper (oper)
    {
        left.setParent(this);
        right.setParent(this);
        _exps.push_back(&left);
        _exps.push_back(&right);
    }

    /** \brief Destroy a Operation Expression node.
    **
    ** Delete left and right, see constructor. */
    virtual ~OpExp ()
    {
    }
    /** \} */

    virtual OpExp* clone()
    {
        OpExp* cloned = new OpExp(getLocation(), *getLeft().clone(), getOper(), *getRight().clone());
        cloned->setVerbose(isVerbose());
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


    /** \name Setters. */
public :
    virtual void setLeft(Exp& left)
    {
        _exps[0] = &left;
        left.setParent(this);
    }

    virtual void setRight(Exp& right)
    {
        _exps[1] = &right;
        right.setParent(this);
    }
    /** \} */


    /** \name Accessors.
    ** \{ */
public:
    /** \brief Return the left expression of the operation (read only) */
    const Exp& getLeft() const
    {
        return *_exps[0];
    }
    /** \brief Return the left expression of the operation (read and write) */
    Exp& getLeft()
    {
        return *_exps[0];
    }

    /** \brief Return the operator description (read only) */
    Oper getOper() const
    {
        return _oper;
    }

    /** \brief Return the right expression of the operation (read only) */
    const Exp& getRight() const
    {
        return *_exps[1];
    }
    /** \brief Return the right expression of the operation (read and write) */
    Exp& getRight()
    {
        return *_exps[1];
    }

    virtual ExpType getType() const
    {
        return OPEXP;
    }
    inline bool isOpExp() const
    {
        return true;
    }

    inline bool isBooleanOp() const
    {
        return _oper == eq || _oper == ne || _oper == lt || _oper == le || _oper == gt || _oper == ge || _oper == logicalAnd || _oper == logicalOr || _oper == logicalShortCutAnd || _oper == logicalShortCutOr;
    }

    inline std::wstring getString() const
    {
        switch (_oper)
        {
            case plus:
                return L"+";
            case minus:
                return L"-";
            case times:
                return L"*";
            case rdivide:
                return L"/";
            case ldivide:
                return L"\\";
            case power:
                return L"^";
            case dottimes:
                return L".*";
            case dotrdivide:
                return L"./";
            case dotldivide:
                return L".\\";
            case dotpower:
                return L".^";
            case krontimes:
                return L".*.";
            case kronrdivide:
                return L"./.";
            case kronldivide:
                return L".\\.";
            case controltimes:
                return L"*.";
            case controlrdivide:
                return L"/.";
            case controlldivide:
                return L"\\.";
            case eq:
                return L"==";
            case ne:
                return L"~=";
            case lt:
                return L"<";
            case le:
                return L"<=";
            case gt:
                return L">";
            case ge:
                return L">=";
            case logicalAnd:
                return L"&";
            case logicalOr:
                return L"|";
            case logicalShortCutAnd:
                return L"&&";
            case logicalShortCutOr:
                return L"||";
            case unaryMinus:
                return L"-";
        }
    }


protected:
    /** \brief Operator. */
    Oper _oper;
};

} // namespace ast

#endif // !AST_OPEXP_HXX
