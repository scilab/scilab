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
              Exp& test, Exp& body)
        : ControlExp (location)
    {
        test.setParent(this);
        body.setParent(this);
        _exps[0] = &test;
        _exps[1] = &body;
    }

    /** \brief Destroy a While Expression node.
    **
    ** Delete test and body (see constructor). */
    virtual ~WhileExp ()
    {
        delete _exps[0];
        delete _exps[1];
    }
    /** \}*/

    virtual WhileExp* clone()
    {
        WhileExp* cloned = new WhileExp(getLocation(), *getTest().clone(), *getBody().clone());
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

    virtual ExpType getType()
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
