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

/**
 ** \file trycatchexp.hxx
 ** Define the Trycatchuence Expression class.
 */

#ifndef AST_TRYCATCHEXP_HXX
#define AST_TRYCATCHEXP_HXX

#include "controlexp.hxx"
#include "seqexp.hxx"

namespace ast
{

/** \brief Abstract a Trycatchuence Expression node.
**
** \b Example: try (print_int(2097); catch print("WipeOut")) */
class TryCatchExp : public ControlExp
{
    /** \name Ctor & dtor.
    ** \{ */
public:
    /** \brief Construct a Trycatchuence Expression node.
    ** \param location scanner position informations
    ** \param body EXP LIST intruction
    */
    TryCatchExp (const Location& location,
                 Exp& tryme,
                 Exp& catchme)
        : ControlExp (location)
    {
        tryme.setParent(this);
        catchme.setParent(this);
        _exps[0] = &tryme;
        _exps[1] = &catchme;
    }

    virtual ~TryCatchExp ()
    {
        delete _exps[0];
        delete _exps[1];
    }

    virtual TryCatchExp* clone()
    {
        TryCatchExp* cloned = new TryCatchExp(getLocation(), *getTry().clone(), *getCatch().clone());
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
    const Exp& getTry() const
    {
        return *_exps[0];
    }

    Exp& getTry()
    {
        return *_exps[0];
    }

    const Exp& getCatch() const
    {
        return *_exps[1];
    }

    Exp& getCatch()
    {
        return *_exps[1];
    }
    /** \} */

    virtual ExpType getType()
    {
        return TRYCATCHEXP;
    }
    inline bool isTryCatchExp() const
    {
        return true;
    }
};

} // namespace ast

#endif /* !AST_TRYCATCHEXP_HXX */
