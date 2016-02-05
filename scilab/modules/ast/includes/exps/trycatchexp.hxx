/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2008-2008 - DIGITEO - Bruno JOFRET
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
                 SeqExp& tryme,
                 SeqExp& catchme)
        : ControlExp (location)
    {
        tryme.setParent(this);
        catchme.setParent(this);
        _exps.push_back(tryme.getAs<Exp>());
        _exps.push_back(catchme.getAs<Exp>());
    }

    virtual ~TryCatchExp ()
    {
    }

    virtual TryCatchExp* clone()
    {
        TryCatchExp* cloned = new TryCatchExp(getLocation(), *getTry().clone()->getAs<SeqExp>(), *getCatch().clone()->getAs<SeqExp>());
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

    virtual ExpType getType() const
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
