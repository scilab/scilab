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

/**
 ** \file visitor.hxx
 ** \brief Defines the root class of all visitor classes.
 **
 ** There are two kinds of visitors, just like there are two kinds of
 ** iterators: the ConstVisitors (which cannot modify the tree they
 ** visit), and the NonConstVisitors (which *can* modify the tree).\n
 ** \n
 ** Both kind of visitors share the same form of hierarchy:
 ** AbstractVisitor (<- AConcreteVisitor).\n
 ** \n
 ** Since we don't want to implement twice the AbstractVisitor and
 ** twice the DefaultVisitor, we use class templates:
 ** \li GenVisitor < const_kind >
 ** \li GenVisitor < non_const_kind >
 */

#ifndef AST_VISITOR_HXX
#define AST_VISITOR_HXX

#include "decls.hxx"
#include "dynlib_ast.h"

namespace ast
{

/** \name Allow const selection. */
/** \{ */
/** \brief Select const methods. */
struct const_kind {};
/** \brief Select non-const methods. */
struct non_const_kind {};
/** \} */

template <typename K, typename T>
/** \brief select_const<SELECTOR, TYPE>.
 **
 ** We use template to be able to select the const or not
 ** const variety of a TYPE.\n
 ** SELECTOR must be `const_kind' or `non_const_kind'. */
struct select_const
{
    /** \brief \a t is non-const, just like T. */
    typedef T t;
};

template <typename T>
/** \brief select_const<const_kind, TYPE>
 **
 ** Specialized version of select_const for const_kind.
 ** \see select_const */
struct select_const<const_kind, T>
{
    /** \brief \a t is the const version of type T. */
    typedef const T t;
};


template <typename K>
/** \brief Root class of all visitors.
 **
 ** GenVisitor<CONSTNESS-SELECTOR> is the root class of all
 ** visitors. */
class EXTERN_AST GenVisitor
{
    /** \name Ctor & dtor.
     ** \{ */
public:
    /** \brief Destroys a GenVisitor */
    virtual ~GenVisitor ()
    {
    }
    /** \} */

    //force devrived class to implement clone method
    virtual GenVisitor* clone() = 0;


    /** \name Visit Variable related nodes.
     ** \{ */
public:
    virtual void visit (typename select_const<K, SimpleVar>::t & e) = 0;
    virtual void visit (typename select_const<K, DollarVar>::t & e) = 0;
    virtual void visit (typename select_const<K, ColonVar>::t & e) = 0;
    virtual void visit (typename select_const<K, ArrayListVar>::t & e) = 0;
    /** \} */

    /** \name Visit Constant Expressions related nodes.
     ** \{ */
public:
    virtual void visit (typename select_const<K, DoubleExp>::t & e) = 0;
    virtual void visit (typename select_const<K, BoolExp>::t & e) = 0;
    virtual void visit (typename select_const<K, StringExp>::t & e) = 0;
    virtual void visit (typename select_const<K, CommentExp>::t & e) = 0;
    virtual void visit (typename select_const<K, NilExp>::t & e) = 0;
    /** \} */

    /** \name Visit Instructions related nodes.
     ** \{ */
public:
    virtual void visit (typename select_const<K, CallExp>::t & e) = 0;
    virtual void visit (typename select_const<K, CellCallExp>::t & e) = 0;
    virtual void visit (typename select_const<K, OpExp>::t & e) = 0;
    virtual void visit (typename select_const<K, LogicalOpExp>::t & e) = 0;
    virtual void visit (typename select_const<K, AssignExp>::t & e) = 0;
    virtual void visit (typename select_const<K, IfExp>::t & e) = 0;
    virtual void visit (typename select_const<K, WhileExp>::t & e) = 0;
    virtual void visit (typename select_const<K, ForExp>::t & e) = 0;
    virtual void visit (typename select_const<K, BreakExp>::t & e) = 0;
    virtual void visit (typename select_const<K, ContinueExp>::t & e) = 0;
    virtual void visit (typename select_const<K, TryCatchExp>::t & e) = 0;
    virtual void visit (typename select_const<K, SelectExp>::t & e) = 0;
    virtual void visit (typename select_const<K, CaseExp>::t & e) = 0;
    virtual void visit (typename select_const<K, ReturnExp>::t & e) = 0;
    virtual void visit (typename select_const<K, FieldExp>::t & e) = 0;
    /** \} */

    /** \name Visit Single Operation related nodes.
     ** \{ */
public:
    virtual void visit (typename select_const<K, NotExp>::t & e) = 0;
    virtual void visit (typename select_const<K, TransposeExp>::t & e) = 0;
    /** \} */

    /** \name Visit Matrix related nodes.
     ** \{ */
public:
    virtual void visit (typename select_const<K, MatrixExp>::t & e) = 0;
    virtual void visit (typename select_const<K, MatrixLineExp>::t & e) = 0;
    /** \} */

    /** \name Visit Cell related nodes.
     ** \{ */
public:
    virtual void visit (typename select_const<K, CellExp>::t & e) = 0;
    /** \} */

    /** \name Visit List of Expressions related nodes.
      ** \{ */
public:
    virtual void visit (typename select_const<K, SeqExp>::t & e) = 0;
    virtual void visit (typename select_const<K, ArrayListExp>::t & e) = 0;
    virtual void visit (typename select_const<K, AssignListExp>::t & e) = 0;
    /** \} */


    /** \name Visit Declaration related nodes.
     ** \{ */
public:
    virtual void visit (typename select_const<K, VarDec>::t & e) = 0;
    virtual void visit (typename select_const<K, FunctionDec>::t & e) = 0;
    /** \} */

    /** \name Visit Type dedicated Expressions related node.
     ** \{ */
public:
    virtual void visit (typename select_const<K, ListExp>::t & e) = 0;
    /** \} */

public:
    virtual void visit (typename select_const<K, OptimizedExp>::t & e) = 0;
    virtual void visit (typename select_const<K, MemfillExp>::t & e) = 0;

    /*optimized*/
    virtual void visit (typename select_const<K, DAXPYExp>::t & e) = 0;

    virtual void visit (typename select_const<K, IntSelectExp>::t & e) = 0;
    virtual void visit (typename select_const<K, StringSelectExp>::t & e) = 0;
};

/** \brief Define shortand type for a const visitor */
typedef GenVisitor<const_kind> ConstVisitor;
/** \brief Define shortand type for a non const visitor */
typedef GenVisitor<non_const_kind> Visitor;

} // namespace ast


#endif // !AST_VISITOR_HXX
