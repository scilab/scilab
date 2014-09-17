/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2006-2008 - DIGITEO - Bruno JOFRET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef AST_IFEXP_HXX
#define AST_IFEXP_HXX

#include <assert.h>
#include "controlexp.hxx"

namespace ast
{
/*
** \brief Abstract an If Expression node.
**
** \b Example: if (2097 = 0) then disp("fail") else disp("OK")
*/
class IfExp : public ControlExp
{
public:
    /*
    ** \brief Construct an If Expression node.
    ** \param location scanner position informations
    ** \param test chose the instruction to execute
    ** \param then instruction if test is true
    ** \param else instruction if test is false
    */
    IfExp(const Location& location,
          Exp& test, Exp& t, Exp& e)
        : ControlExp (location),
          _test (&test),
          _then (&t),
          _else (&e),
          _hasElse (true)
    {
    }

    IfExp(const Location& location,
          Exp& test, Exp& t)
        : ControlExp (location),
          _test (&test),
          _then (&t),
          _else (new ast::CommentExp(location, new std::wstring(L"No else !!"))),
          // For first display in order to see what appends
          _hasElse (false)
    {
    }

    virtual ~IfExp()
    {
        delete _test;
        delete _then;
        delete _else;
    }

    virtual IfExp* clone()
    {
        IfExp* cloned = new IfExp(getLocation(), *getTest().clone(), *getThen().clone(), *getElse().clone());
        cloned->setVerbose(isVerbose());
        return cloned;
    }

    // \brief Visitors entry point.
public:
    virtual void	accept(Visitor& v)
    {
        v.visit (*this);
    }
    virtual void	accept(ConstVisitor& v) const
    {
        v.visit (*this);
    }

    // \brief Accessors.
public:
    // \brief Return the select condition of the loop (read only).
    const Exp&	getTest() const
    {
        return *_test;
    }
    // \brief Return the select condition of the loop (read and write).
    Exp& getTest()
    {
        return *_test;
    }

    // \brief Return the intructions if test is true (read only).
    const Exp&	getThen() const
    {
        return *_then;
    }
    // \brief Return the instructions if test is true (read and write).
    Exp& getThen()
    {
        return *_then;
    }

    // \brief Return the instruction if test is false (read only).
    const Exp& getElse() const
    {
        return *_else;
    }
    // \brief Return the instruction if test is false (read and write).
    Exp& getElse()
    {
        return *_else;
    }

    // \brief Return if there is an else body
    bool	hasElse()
    {
        return _hasElse;
    }
    // \brief Return if there is an else body
    bool	hasElse() const
    {
        return _hasElse;
    }

    virtual ExpType getType()
    {
        return IFEXP;
    }
    inline bool isIfExp() const
    {
        return true;
    }
protected:
    // \brief "has a value" qualifier.
    Exp*	_test;
    Exp*	_then;
    Exp*	_else;
    bool	_hasElse;
};

} // namespace ast

#endif // !AST_IFEXP_HXX
