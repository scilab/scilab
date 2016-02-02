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
 ** \file ast/whileexp.hxx
 ** Define the While Expression class.
 */

#ifndef AST_WHILEEXP_HXX
#define AST_WHILEEXP_HXX

#include "controlexp.hxx"

namespace ast
{
/** \brief Abstract a For Expression node.
**
** \b Example: while (2097 <> 1) do print("WipeOut") */
class WhileExp : public ControlExp
{
    /** \name Ctor & dtor.
    ** \{ */
public:
    /** \brief Construct a While Expression node.
    ** \param location scanner position informations
    ** \param test exit condition of the loop
    ** \param body instructions executed in the loop
    **
    ** \b Example: while (2097 <> 1) do print("WipeOut")
    ** \li "(2097 <> 1)" is the exit condition (allways true here)
    ** \li "print("WipeOut")" body of the loop
    */
    WhileExp (const Location& location,
              Exp& test, SeqExp& body)
        : ControlExp (location)
    {
        test.setParent(this);
        body.setParent(this);
        _exps.push_back(&test);
        _exps.push_back(body.getAs<Exp>());

        //set capabilities flags @init instead of each calls.
        body.setBreakable();
        body.setContinuable();
    }

    /** \brief Destroy a While Expression node.
    **
    ** Delete test and body (see constructor). */
    virtual ~WhileExp ()
    {
    }
    /** \}*/

    virtual WhileExp* clone()
    {
        WhileExp* cloned = new WhileExp(getLocation(), *getTest().clone(), *getBody().clone()->getAs<SeqExp>());
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
    /** \brief Return the exit condition of the loop (read only). */
    const Exp& getTest() const
    {
        return *_exps[0];
    }
    /** \brief Return the exit condition of the loop (read and write). */
    Exp& getTest()
    {
        return *_exps[0];
    }

    /** \brief Return the body of the loop (read only). */
    const Exp& getBody() const
    {
        return *_exps[1];
    }
    /** \brief Return the body of the loop (read and write). */
    Exp& getBody()
    {
        return *_exps[1];
    }
    /** \} */

    virtual ExpType getType() const
    {
        return WHILEEXP;
    }
    inline bool isWhileExp() const
    {
        return true;
    }
};

} // namespace ast

#endif // !AST_WHILEEXP_HXX
