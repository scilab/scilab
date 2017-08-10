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
 ** \file forexp.hxx
 ** Define the For Expression class.
 */

#ifndef AST_FOREXP_HXX
#define AST_FOREXP_HXX

#include "controlexp.hxx"
#include "vardec.hxx"

namespace ast
{

/** \brief Abstract a For Expression node.
**
** \b Example: for i = 0:2097 do print("WipeOut") */
class ForExp : public ControlExp
{
    /** \name Ctor & dtor.
    ** \{ */
public:
    /** \brief Construct a For Expression node.
    ** \param location scanner position informations
    ** \param vardec implicit variable declaration
    ** \param body instructions executed in the for loop
    **
    ** \b Example: for i = 0:2097 do print("WipeOut")
    ** \li "i = 0:2097" is the variable declaration
    ** \li "print("WipeOut")" is the body
    */
    ForExp (const Location& location,
            Exp& vardec, SeqExp& body)
        : ControlExp (location)
    {
        vardec.setParent(this);
        body.setParent(this);
        _exps.push_back(&vardec);
        _exps.push_back(body.getAs<Exp>());

        body.setBreakable();
        body.setContinuable();
    }

    /** \brief Destroy a For Expression node.
    **
    ** Delete vardec, hi and body (see constructor). */
    virtual ~ForExp ()
    {
    }
    /** \} */

    virtual ForExp* clone()
    {
        ForExp* cloned = new ForExp(getLocation(), *getVardec().clone(), *getBody().clone()->getAs<SeqExp>());
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
    /** \brief Return the implicit variable declaration (read only) */
    Exp& getVardec() const
    {
        return *_exps[0];
    }
    /** \brief Return the implicit variable declaration (read and write) */
    Exp& getVardec()
    {
        return *_exps[0];
    }

    /** \brief Return the body of the loop (read only) */
    const Exp& getBody() const
    {
        return *_exps[1];
    }
    /** \brief Return the body of the loop (read and write) */
    Exp& getBody()
    {
        return *_exps[1];
    }
    /** \} */

    virtual ExpType getType() const
    {
        return FOREXP;
    }
    inline bool isForExp() const
    {
        return true;
    }
};

} // namespace ast

#endif // !AST_FOREXP_HXX
