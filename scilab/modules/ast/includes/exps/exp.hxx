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
** \file exp.hxx
** Define the Expression mother class.
*/

#ifndef AST_EXP_HXX
#define AST_EXP_HXX

#include <list>

#include "ast.hxx"

namespace ast
{

/** \brief Abstract an Expression node. */
class Exp : public Ast
{

    /** \name Ctor & dtor.
    ** \{ */
public:
    /** \brief Construct an Expression node.
    ** \param location scanner position informations */
    Exp (const Location& location)
        : Ast (location),
          _verbose(false),
          _bBreak(false),
          _bBreakable(false),
          _bReturn(false),
          _bReturnable(false),
          _bContinue(false),
          _bContinuable(false)
    {
    }
    /** \brief Destroys an Expression node. */
    virtual ~Exp ()
    {
    }
    /** \} */

    virtual Exp* clone() = 0;

public:
    /** \brief Return if an expression should be displayed or not. */
    inline void mute(void)
    {
        _verbose = false;
    }

    /** \brief Return if an expression should be displayed or not. */
    inline void set_verbose(bool verbose)
    {
        _verbose = verbose;
    }

    /** \brief Return if an expression should be displayed or not. */
    inline bool is_verbose(void) const
    {
        return _verbose;
    }

    inline void break_set(void)
    {
        _bBreak = true;
    }

    inline void break_reset(void)
    {
        _bBreak = false;
    }

    inline bool is_break(void) const
    {
        return _bBreak;
    }

    inline void breakable_set(void)
    {
        _bBreakable = true;
    }

    inline void breakable_reset(void)
    {
        _bBreakable = false;
    }

    inline bool is_breakable(void) const
    {
        return _bBreakable;
    }

    inline void return_set(void)
    {
        _bReturn = true;
    }

    inline void return_reset(void)
    {
        _bReturn = false;
    }

    inline bool is_return(void) const
    {
        return _bReturn;
    }

    inline void returnable_set(void)
    {
        _bReturnable = true;
    }

    inline void returnable_reset(void)
    {
        _bReturnable = false;
    }

    inline bool is_returnable(void) const
    {
        return _bReturnable;
    }

    inline void continue_set(void)
    {
        _bContinue = true;
    }

    inline void continue_reset(void)
    {
        _bContinue = false;
    }

    inline bool is_continue(void) const
    {
        return _bContinue;
    }

    inline void continuable_set(void)
    {
        _bContinuable = true;
    }

    inline void continuable_reset(void)
    {
        _bContinuable = false;
    }

    inline bool is_continuable(void) const
    {
        return _bContinuable;
    }

    inline virtual bool is_simple_var() const
    {
        return false;
    }

    inline virtual bool is_assign_exp() const
    {
        return false;
    }

    inline virtual bool is_return_exp() const
    {
        return false;
    }

    inline virtual bool is_cellcall_exp() const
    {
        return false;
    }

private:
    bool _verbose;
    bool _bBreak;
    bool _bBreakable;
    bool _bReturn;
    bool _bReturnable;
    bool _bContinue;
    bool _bContinuable;
};

/** \brief Define a shorthand for list of Exp* manipulation. */
typedef std::list<Exp *> exps_t;

} // namespace ast

#endif // !AST_EXP_HXX
