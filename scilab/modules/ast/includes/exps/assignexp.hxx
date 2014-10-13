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
               Exp& right_exp,
               types::InternalType* pIT = NULL)
        : Exp (location),
          lr_owner(true),
          _pIT(pIT)
    {
        left_exp.setParent(this);
        right_exp.setParent(this);
        _exps.push_back(&left_exp);
        _exps.push_back(&right_exp);
    }
    /** \brief Destroy an Assign Exp node.
    **
    ** Delete var and exp (see constructor). */
    virtual ~AssignExp ()
    {
        if (lr_owner == false)
        {
            //set to NULL to avoid delete in ~Exp()
            _exps[0] = NULL;
            _exps[1] = NULL;
        }
    }
    /** \} */

    virtual AssignExp* clone()
    {
        AssignExp* cloned = new AssignExp(getLocation(), *getLeftExp().clone(), *getRightExp().clone());
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


    /** \name Accessors.
    ** \{ */
public:
    /** \brief Return the name of the size (read only). */
    Exp& getLeftExp() const
    {
        return *_exps[0];
    }
    /** \brief Return the name of the size */
    Exp& getLeftExp()
    {
        return *_exps[0];
    }

    /** \brief Return the name of the init (read only). */
    Exp& getRightExp() const
    {
        return *_exps[1];
    }
    /** \brief Return the name of the init */
    Exp& getRightExp()
    {
        return *_exps[1];
    }

    types::InternalType* getRightVal()
    {
        return _pIT;
    }

    types::InternalType* getRightVal() const
    {
        return _pIT;
    }

    void setLrOwner(const bool b)
    {
        lr_owner = b;
    }

    /** \} */

    virtual ExpType getType()
    {
        return ASSIGNEXP;
    }
    inline bool isAssignExp() const
    {
        return true;
    }

protected:
    bool lr_owner;
    types::InternalType* _pIT;
};
} // namespace ast

#endif // !AST_ASSIGNEXP_HXX
