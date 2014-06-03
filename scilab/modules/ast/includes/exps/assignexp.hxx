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

#ifndef AST_ASSIGNEXP_HXX
#define AST_ASSIGNEXP_HXX

#include "exp.hxx"
#include "internal.hxx"

namespace ast
{
/** \brief Abstract an Assign Expression node.
**
** \b Example: b = a */
class AssignExp : public Exp
{
public:
    AssignExp (const Location& location,
               Exp& left_exp,
               Exp& right_exp)
        : Exp (location),
          _left_exp (&left_exp),
          _right_exp (&right_exp),
          lr_owner(true),
          _pIT(NULL)
    {
    }

    AssignExp (const Location& location,
               Exp& left_exp,
               Exp& right_exp,
               types::InternalType* pIT)
        : Exp (location),
          _left_exp (&left_exp),
          _right_exp (&right_exp),
          lr_owner(true),
          _pIT(pIT)
    {
    }
    /** \brief Destroy an Assign Exp node.
    **
    ** Delete var et exp (see constructor). */
    virtual ~AssignExp ()
    {
        if (lr_owner)
        {
            delete  _left_exp;
            delete  _right_exp;
        }
    }
    /** \} */

    virtual AssignExp* clone()
    {
        AssignExp* cloned = new AssignExp(location_get(), *left_exp_get().clone(), *right_exp_get().clone());
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


    /** \name Accessors.
    ** \{ */
public:
    /** \brief Return the name of the size (read only). */
    Exp& left_exp_get() const
    {
        return *_left_exp;
    }
    /** \brief Return the name of the size */
    Exp& left_exp_get()
    {
        return *_left_exp;
    }

    /** \brief Return the name of the init (read only). */
    Exp& right_exp_get() const
    {
        return *_right_exp;
    }
    /** \brief Return the name of the init */
    Exp& right_exp_get()
    {
        return *_right_exp;
    }

    types::InternalType* right_val_get()
    {
        return _pIT;
    }

    types::InternalType* right_val_get() const
    {
        return _pIT;
    }

    inline bool is_assign_exp() const
    {
        return true;
    }

    void set_lr_owner(const bool b)
    {
        lr_owner = b;
    }

    /** \} */

protected:
    /** \brief Left variable which is affected. */
    Exp* _left_exp;
    /** \brief Right expression which affect var. */
    Exp* _right_exp;
    types::InternalType* _pIT;
    bool lr_owner;
};
} // namespace ast

#endif // !AST_ASSIGNEXP_HXX
